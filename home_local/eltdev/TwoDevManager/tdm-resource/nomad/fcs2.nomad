job "fcs2" {
    datacenters = ["dc1"]
    
    group "fcs2_group" {
        network {
            port "pub" {}   // random port for pub/sub
            port "req" {}   // random port for req/rep
        }

        // register pub/sub service in Consul
        service {
            name = "fcs2-pub"
            port = "pub"
        }

        // register req/rep service in Consul
        service {
            name = "fcs2-req"
            port = "req"
        }

        task "fcs2" {
            driver = "raw_exec"

            // generate configuration file from template before starting service
	     template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/fcs2.yaml.tpl"
                destination = "local/fcs2.yaml"
            }
	    
            template {
                source = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/lamp1.yaml.tpl"
                destination = "local/lamp1.yaml"
            }

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " fcsDevmgr -c local/fcs2.yaml "]
            }
        }
    }
}
