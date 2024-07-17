job "tdmccf" {
    datacenters = ["dc1"]
    
    group "tdmccf_group" {
        network {
            port "pub" {}   // random port for pub/sub
            port "req" {}   // random port for req/rep
        }

        // register pub/sub service in Consul
        service {
            name = "tdmccf-pub"
            port = "pub"
        }

        // register req/rep service in Consul
        service {
            name = "tdmccf-req"
            port = "req"
        }

        task "tdmccf" {
            driver = "raw_exec"

            // generate configuration file from template before starting service
            template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/ccfDdt.yaml.tpl"
                destination = "local/ccfDdt.yaml"
            }

            template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/ccfSetup.yaml.tpl"
                destination = "local/ccfSetup.yaml"
            }

            config {
              command = "/bin/bash"
               args = ["-l", "-c", " ccfCtrlSim  -c local/ccfDdt.yaml "]
            }
        }
    }
}
