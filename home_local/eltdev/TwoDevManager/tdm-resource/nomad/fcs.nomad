job "fcs" {
    datacenters = ["dc1"]
    
    group "fcs_group" {
        network {
            port "pub" {}   // random port for pub/sub
            port "req" {}   // random port for req/rep
        }

        // register pub/sub service in Consul
        service {
            name = "fcs-pub"
            port = "pub"
        }

        // register req/rep service in Consul
        service {
            name = "fcs-req"
            port = "req"
        }

        task "fcs" {
            driver = "raw_exec"

            // generate configuration file from template before starting service
            template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/fcs.yaml.tpl"
                destination = "local/fcs.yaml"
            }

            template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/shutter1.yaml.tpl"
                destination = "local/shutter1.yaml"
            }

            

            template {
                 source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/iodev1.yaml.tpl"
                 destination = "local/iodev1.yaml"
            }
	    
	    template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/motor1.yaml.tpl"
                destination = "local/motor1.yaml"
            }

            template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/mos1.yaml.tpl"
                destination = "local/mos1.yaml"
            }

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " fcsDevmgr -c local/fcs.yaml "]
            }
        }
    }
}
