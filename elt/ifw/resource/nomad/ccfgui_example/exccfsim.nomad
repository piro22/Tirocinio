job "exccfsim" {
    datacenters = ["dc1"]

    group "exccfsim_group" {
        network {
            port "pub" {}   // random port for pub/sub
            port "req" {}   // random port for req/rep
        }

        // register pub/sub service in Consul
        service {
            name = "exccfsim-pub"
            port = "pub"
        }

        // register req/rep service in Consul
        service {
            name = "exccfsim-req"
            port = "req"
        }

        task "exccfsim" {
            driver = "raw_exec"

            // generate configuration file from template before starting service
            template {
                source = "/home_local/eltdev/NGCII/INTROOT/resource/nomad/ccfgui_example/exccfsim.cfg.yaml.tpl"
                destination = "local/exccfsim.cfg.yaml"
            }

            template {
                source = "/home_local/eltdev/NGCII/INTROOT/resource/nomad/ccfgui_example/exccfsimSetup.yaml.tpl"
                destination = "local/exccfsimSetup.yaml"
            }

            config {
              command = "/bin/bash"
               args = ["-l", "-c", " ccfCtrlSim  -c local/exccfsim.cfg.yaml -l DEBUG"]
            }
        }
    }
}
