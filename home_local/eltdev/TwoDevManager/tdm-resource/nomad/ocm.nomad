job "ocm" {
    datacenters = ["dc1"]
    
    group "ocm_group" {
        network {
            port "pub" {}   // random port for pub/sub
            port "req" {}   // random port for req/rep
        }

        // register pub/sub service in Consul
        service {
            name = "ocm-pub"
            port = "pub"
        }

        // register req/rep service in Consul
        service {
            name = "ocm-req"
            port = "req"
        }

        task "ocm" {
            driver = "raw_exec"

            // generate configuration file from template before starting service
            template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/ocm.yaml.tpl"
                destination = "local/ocm.yaml"
            }

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " daqOcmServer -c local/ocm.yaml"]
            }
        }
    }
}
