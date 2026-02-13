module FprimeSyncbench {
    @ Tcp driver that connects outgoing data to a server
    active component TcpDriver {


        async input port dataIn: BenchDataPort

        async command CONNECT_TCP(hostname: string, hostport: U16)

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

        ###############################################################################
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