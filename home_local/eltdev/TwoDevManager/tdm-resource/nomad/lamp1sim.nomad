job "lamp1sim" {
    datacenters = ["dc1"]
    
    group "lamp1sim_group" {
        network {
            port "lamp1" {} // use random port for lamp simulator
        }

        // register lamp1_sim_sim service in Consul
        service {
            name = "lamp1sim"
            port = "lamp1"
        }

        task "lamp1sim" {
            driver = "raw_exec"

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " fcfDevsimLamp --port ${NOMAD_PORT_lamp1} --cfg config/fcs/devsim/lamp1.cfg.yaml --use-ext-ip"]
            }
        }
    }
}
