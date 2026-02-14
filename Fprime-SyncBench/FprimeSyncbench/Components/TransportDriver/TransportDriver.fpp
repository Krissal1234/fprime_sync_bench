module FprimeSyncbench {

    @ TCP and UDP transmission to external connector for digital twinning
    active component TransportDriver {

        @ Input port for sending data to external server
        async input port dataIn: BenchDataPort

        @ Command to open port for TCP or UDP connection
        async command CONNECT_DRIVER(protocol: TransportProtocol,  hostname: string, hostport: U16)

        @ Allocation port for a buffer
        output port allocate: Fw.BufferGet

        @ Deallocation port for buffers
        output port deallocate: Fw.BufferSend

        @ Allocation failed event
        event MemoryAllocationFailed() severity warning low id 0 format "Failed to allocate memory"

        event SocketError(
            status: I32 @< The error code returned by the socket operation
        ) \
        severity warning high \
        id 2 \
        format "Socket error occurred with status: {}"

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