job "syssup" {
    datacenters = ["dc1"]
    
    group "syssup_group" {
        network {
            port "pub" {}   // random port for pub/sub
            port "req" {}   // random port for req/rep
        }

        // register pub/sub service in Consul
        service {
            name = "syssup-pub"
            port = "pub"
        }

        // register req/rep service in Consul
        service {
            name = "syssup-req"
            port = "req"
        }

        task "syssup" {
            driver = "raw_exec"

            // generate configuration file from template before starting service
            template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/syssup.yaml.tpl"
                destination = "local/syssup.yaml"
                change_mode   = "signal"
                change_signal = "SIGUSR1"
            }

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " supSupervisor -c local/syssup.yaml "]
            }
        }
    }
}
