module FprimeSyncbench {


    enum TransportType {
      TLM @< Telemetry
      EVT @< Event
      BUF @< Buffer
    }

    enum TransportProtocol {
      TCP = 0,
      UDP = 1
    }

    struct BenchData {
        sequence_num: U32
        transport_type : TransportType
        timestamp: U64
    }

    port BenchDataPort(data: BenchData)
}
