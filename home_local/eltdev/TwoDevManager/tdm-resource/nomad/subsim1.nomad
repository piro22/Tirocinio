job "subsim1" {
    datacenters = ["dc1"]
    
    group "subsim1_group" {
        network {
            port "pub" {}   // random port for pub/sub
            port "req" {}   // random port for req/rep
        }

        // register pub/sub service in Consul
        service {
            name = "subsim1-pub"
            port = "pub"
        }

        // register req/rep service in Consul
        service {
            name = "subsim1-req"
            port = "req"
        }

        task "subsim1" {
            driver = "raw_exec"

            // generate configuration file from template before starting service
            template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/subsim1.yaml.tpl"
                destination = "local/subsim1.yaml"
            }

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " supSimulator -c local/subsim1.yaml"]
            }
        }
    }
}
