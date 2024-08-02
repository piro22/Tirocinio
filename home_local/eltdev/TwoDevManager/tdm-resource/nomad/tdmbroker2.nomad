job "tdmbroker2" {
    datacenters = ["dc1"]
    
    group "tdmbroker2_group" {
        network {
            port "broker" {} // use random port for broker
        }

        // register broker service in Consul
        service {
            name = "tdmbroker2"
            port = "broker"
        }

        task "tdmbroker2" {
            driver = "raw_exec"

            config {
                 command = "/bin/bash"
                 args = ["-l", "-c", " ddtBroker --uri zpb.rr://*:${NOMAD_PORT_broker}/broker"]
            }
        }
    }
}
