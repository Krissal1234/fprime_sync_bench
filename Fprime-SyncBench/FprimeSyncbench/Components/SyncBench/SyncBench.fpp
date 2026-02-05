module FprimeSyncbench {

    struct BenchData {
        sequence_num: U32
        transport_type : TransportType
        timestamp: U64
    }

    @ Benchmarking component that will send three data transport patterns to a mock simulation engine to test latency.
    active component SyncBench {

        @ Begin Telemetry burst command
        async command BENCH_TLM(num_runs: U32)

        @ Begin Event burst command
        async command BENCH_EVT(num_runs: U32)

        @ Begin Buffer burst command
        async command BENCH_BUF(num_runs: U32)

        @ Telemtry Sender
        telemetry TelemtrySender: BenchData

        @ EventSender
        event EventSender(data: BenchData) \
        severity activity high id 1\
        format "Type: {}"

        @ Buffer Sender
        output port BufferSender: Fw.BufferSend


        @ Example port: receiving calls from the rate group
        sync input port run: Svc.Sched

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}