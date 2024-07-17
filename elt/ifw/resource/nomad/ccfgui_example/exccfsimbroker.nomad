job "exccfsimbroker" {
    datacenters = ["dc1"]

    group "exccfsimbroker_group" {
        network {
            port "broker" {} // use random port for broker
        }

        // register broker service in Consul
        service {
            name = "exccfsimbroker"
            port = "broker"
        }

        task "exccfsimbroker" {
            driver = "raw_exec"

            config {
                 command = "/bin/bash"
                 args = ["-l", "-c", " ddtBroker --uri zpb.rr://*:${NOMAD_PORT_broker}/broker"]
            }
        }
    }
}
