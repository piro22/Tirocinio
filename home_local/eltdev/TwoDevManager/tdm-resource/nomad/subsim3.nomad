job "subsim3" {
    datacenters = ["dc1"]
    
    group "subsim3_group" {
        network {
            port "pub" {}   // random port for pub/sub
            port "req" {}   // random port for req/rep
        }

        // register pub/sub service in Consul
        service {
            name = "subsim3-pub"
            port = "pub"
        }

        // register req/rep service in Consul
        service {
            name = "subsim3-req"
            port = "req"
        }

        task "subsim3" {
            driver = "raw_exec"

            // generate configuration file from template before starting service
            template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/subsim3.yaml.tpl"
                destination = "local/subsim3.yaml"
            }

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " supSimulator -c local/subsim3.yaml"]
            }
        }
    }
}
