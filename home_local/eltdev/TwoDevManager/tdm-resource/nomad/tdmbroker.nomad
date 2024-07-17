job "tdmbroker" {
    datacenters = ["dc1"]
    
    group "tdmbroker_group" {
        network {
            port "broker" {} // use random port for broker
        }

        // register broker service in Consul
        service {
            name = "tdmbroker"
            port = "broker"
        }

        task "tdmbroker" {
            driver = "raw_exec"

            config {
                 command = "/bin/bash"
                 args = ["-l", "-c", " ddtBroker --uri zpb.rr://*:${NOMAD_PORT_broker}/broker"]
            }
        }
    }
}
