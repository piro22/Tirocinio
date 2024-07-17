job "mos1sim" {
    datacenters = ["dc1"]
    
    group "mos1sim_group" {
        network {
            port "mos1" {} // use random port for lamp simulator
        }

        // register lamp1_sim_sim service in Consul
        service {
            name = "mos1sim"
            port = "mos1"
        }

        task "mos1sim" {
            driver = "raw_exec"

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " fcsDevsimMos --port ${NOMAD_PORT_mos1} --cfg config/fcs/devsim/mos/mos1.cfg.yaml --use-ext-ip"]
            }
        }
    }
}
