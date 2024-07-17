job "subsim2" {
    datacenters = ["dc1"]
    
    group "subsim2_group" {
        network {
            port "pub" {}   // random port for pub/sub
            port "req" {}   // random port for req/rep
        }

        // register pub/sub service in Consul
        service {
            name = "subsim2-pub"
            port = "pub"
        }

        // register req/rep service in Consul
        service {
            name = "subsim2-req"
            port = "req"
        }

        task "subsim2" {
            driver = "raw_exec"

            // generate configuration file from template before starting service
            template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/subsim2.yaml.tpl"
                destination = "local/subsim2.yaml"
            }

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " supSimulator -c local/subsim2.yaml"]
            }
        }
    }
}
