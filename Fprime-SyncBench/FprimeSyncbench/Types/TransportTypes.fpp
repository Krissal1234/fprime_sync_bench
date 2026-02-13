module FprimeSyncbench {


    enum TransportType{
      TLM @< Telemetry
      EVT @< Event
      BUF @< Buffer
    }

    struct BenchData {
        sequence_num: U32
        transport_type : TransportType
        timestamp: U64
    }

    port BenchDataPort(data: BenchData)
}
