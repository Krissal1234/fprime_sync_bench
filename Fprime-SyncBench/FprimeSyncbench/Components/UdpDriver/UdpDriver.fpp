module FprimeSyncbench {
    @ Udp Driver to connect to the mock server in order to test latencies for digital twinning
    active component UdpDriver {


        @ Input port to receive data to be sent over UDP
        async input port dataIn: BenchDataPort

        @ Command to open UDP socket
        async command CONNECT_UDP(hostname: string, hostport: U16)

        @ Allocation port for a buffer
        output port allocate: Fw.BufferGet

        @ Deallocation port for buffers
        output port deallocate: Fw.BufferSend

        @ Allocation failed event
        event MemoryAllocationFailed() severity warning low id 0 format "Failed to allocate memory"

        @ Buffer Event
        event BufferEvent(data: BenchData) \
        severity activity high id 1\
        format "{}"

        ##########        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

        @ Enables event handling
        import Fw.Event

    }
}