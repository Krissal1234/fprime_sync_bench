import time
import csv
import socket
import struct
import threading

number_of_runs = 100
tlm_channel_name = "FprimeSyncbench.syncBench.TelemtrySender"
evt_name = "FprimeSyncbench.syncBench.EventSender"
fieldnames = ["sequence", "transport_type", "fsw_timestamp_sec", "arrival_timestamp_sec", "latency_ms"]

server_addr = ('127.0.0.1', 10000)

### ------------------------- Telemetry Test -------------------------------

def test_benchmark_telemetry(fprime_test_api):
    results_buffer = []
    fprime_test_api.clear_histories()

    fprime_test_api.send_command("FprimeSyncbench.syncBench.BENCH_TLM", [number_of_runs])

    for i in range(number_of_runs):
        result = fprime_test_api.await_telemetry(tlm_channel_name)
        data = result.get_val()
        process_buffer(data, results_buffer)

    csv_file = "TLM_bench_results.csv"
    write_to_csv(csv_file, results_buffer)

    assert len(results_buffer) == number_of_runs

# ### ------------------------- Events Test -------------------------------

def test_benchmark_events(fprime_test_api):
    results_buffer = []
    fprime_test_api.clear_histories()

    fprime_test_api.send_command("FprimeSyncbench.syncBench.BENCH_EVT", [number_of_runs])

    for i in range(number_of_runs):
        result = fprime_test_api.await_event(evt_name)

        # getting data from events differs from tlm
        args = result.get_args()
        bench_data_obj = args[0]

        data = bench_data_obj.val
        process_buffer(data, results_buffer)

    csv_file = "EVT_bench_results.csv"
    write_to_csv(csv_file, results_buffer)

    assert len(results_buffer) == number_of_runs


### ------------------------- Buffer Connection Test -------------------------------

def test_benchmark_buffer_udp(fprime_test_api):
    results_buffer = []
    fprime_test_api.clear_histories()

    server_thread = threading.Thread(target=start_buffer_server, args=(results_buffer, number_of_runs, True))
    server_thread.daemon = True
    server_thread.start()

    time.sleep(2)

    fprime_test_api.send_command("FprimeSyncbench.transportDriver.CONNECT_DRIVER", ["UDP", server_addr[0], server_addr[1]])

    time.sleep(2)

    fprime_test_api.send_command("FprimeSyncbench.syncBench.BENCH_BUF", [number_of_runs])

    server_thread.join(timeout=number_of_runs * 2)

    csv_file = "UDP_buffer_bench_results.csv"
    write_to_csv(csv_file, results_buffer)

    assert len(results_buffer) == number_of_runs


def test_benchmark_buffer_tcp(fprime_test_api):

    results_buffer = []
    fprime_test_api.clear_histories()

    server_thread = threading.Thread(target=start_buffer_server, args=(results_buffer, number_of_runs))
    server_thread.daemon = True
    server_thread.start()

    time.sleep(2)

    fprime_test_api.send_command("FprimeSyncbench.transportDriver.CONNECT_DRIVER", ["TCP", server_addr[0], server_addr[1]])

    time.sleep(2)

    fprime_test_api.send_command("FprimeSyncbench.syncBench.BENCH_BUF", [number_of_runs])

    server_thread.join(timeout= number_of_runs * 2)

    csv_file = "TCP_buffer_bench_results.csv"

    write_to_csv(csv_file, results_buffer)

    assert len(results_buffer) == number_of_runs


###------------------------------ Helper Functions -----------------------------------

def start_buffer_server(shared_list, expected_count, use_udp = False):
    sock_type = socket.SOCK_DGRAM if use_udp else socket.SOCK_STREAM
    server_socket = socket.socket(socket.AF_INET, sock_type)
    receive_count = 0

    server_socket.bind(server_addr)

    if not use_udp:
        server_socket.listen(1)

    try:
        if not use_udp:
            # TCP Logic:
            connection, client_addr = server_socket.accept()
            while receive_count < expected_count:
                data = connection.recv(16)
                if not data: break
                print(f"TCP Received packet {receive_count} from {client_addr}")
                process_packet(data, shared_list)
                receive_count += 1
            connection.close()
        else:
            # UDP Logic:
            while receive_count < expected_count:
                data, client_addr = server_socket.recvfrom(16)
                print(f"UDP Received packet {receive_count} from {client_addr}")
                process_packet(data, shared_list)
                receive_count += 1
    except Exception as e:
        print(f"Server error: {e}")
    finally:
        server_socket.close()

def process_packet(data, shared_list):
    arrival_time = time.time()

    unpacked = struct.unpack(">IiQ", data)
    fsw_time_secs = unpacked[2] / 1000000.0

    shared_list.append({
        "sequence": unpacked[0],
        "transport_type": unpacked[1],
        "fsw_timestamp_sec": fsw_time_secs,
        "arrival_timestamp_sec": arrival_time,
        "latency_ms": (arrival_time - fsw_time_secs) * 1000
    })

def process_buffer(data_values, results_buffer):
    arrival_time = time.time()
    fsw_time_usecs = int(data_values["timestamp"])
    fsw_time_secs = fsw_time_usecs / 1000000.0

    results_buffer.append({
        "sequence": data_values["sequence_num"],
        "transport_type": data_values["transport_type"],
        "fsw_timestamp_sec": fsw_time_secs,
        "arrival_timestamp_sec": arrival_time,
        "latency_ms": (arrival_time - fsw_time_secs) * 1000
    })


def write_to_csv(csv_file, results_buffer):

    with open(csv_file, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(results_buffer)

    print(f"Benchmark Finished, saved {len(results_buffer)} results to {csv_file}")




## ------------------------- Write to Telemetry Database time --------------------------------
def benchmark_telemetry_chnl_time(fprime_test_api):
    # This is only testing the time it takes for fprime to move data from tlmWrite to Svc.TlmChan ( which is the telemetry database, so its negligble)
    number_of_runs = 35
    channel = "FprimeSyncbench.syncBench.TelemtrySender"

    telem_seq = [channel] * number_of_runs

    fprime_test_api.clear_histories()

    results =  fprime_test_api.send_and_await_telemetry("FprimeSyncbench.syncBench.BENCH_TLM", [number_of_runs], telem_seq, timeout=10)

    results_buffer = []
    for entry in results:

        time_obj = entry.get_time()
        arrival_time = time_obj.seconds + (time_obj.useconds / 1000000.0)

        val = entry.get_val()
        fsw_time_usecs = int(val["timestamp"])
        fsw_time_secs = fsw_time_usecs / 1000000.0

        results_buffer.append({
            "sequence": val["sequence_num"],
            "transport_type": val["transport_type"],
            "fsw_timestamp_sec": fsw_time_secs,
            "arrival_timestamp_sec": arrival_time,
            # (Arrival in sec - Departure in sec) * 1000 - Latency in Milliseconds
            "latency_ms": (arrival_time - fsw_time_secs) * 1000
        })

    csv_file = "CHNL_TIME_benchmark_results.csv"
    write_to_csv(csv_file)

    print(f"Benchmark Complete. Captured {len(results)} packets.")
