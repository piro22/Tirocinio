job "ccsup" {
    datacenters = ["dc1"]
    
    group "ccssup_group" {
        network {
            port "pub" {}   // random port for pub/sub
            port "req" {}   // random port for req/rep
        }

        // register pub/sub service in Consul
        service {
            name = "ccssup-pub"
            port = "pub"
        }

        // register req/rep service in Consul
        service {
            name = "ccssup-req"
            port = "req"
        }

        task "ccssup" {
            driver = "raw_exec"

            // generate configuration file from template before starting service
            template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/ccssup.yaml.tpl"
                destination = "local/ccssup.yaml"
            }

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " supSupervisor -c local/ccssup.yaml -l INFO"]
            }
        }
    }
}
