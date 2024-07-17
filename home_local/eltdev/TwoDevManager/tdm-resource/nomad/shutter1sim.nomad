job "shutter1sim" {
    datacenters = ["dc1"]
    
    group "shutter1sim_group" {
        network {
            port "shutter1" {} // use random port for redis
        }

        // register shutter1_sim_sim service in Consul
        service {
            name = "shutter1sim"
            port = "shutter1"
        }

        task "shutter1_sim" {
            driver = "raw_exec"

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " fcfDevsimShutter --port ${NOMAD_PORT_shutter1} --cfg config/fcs/devsim/shutter1.cfg.yaml --use-ext-ip"]
            }
        }
    }
}
