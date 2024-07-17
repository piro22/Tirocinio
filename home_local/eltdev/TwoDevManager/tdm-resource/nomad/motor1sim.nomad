job "motor1sim" {
    datacenters = ["dc1"]
    
    group "motor1sim_group" {
        network {
            port "motor1" {} // use random port for redis
        }

        // register motor1sim service in Consul
        service {
            name = "motor1sim"
            port = "motor1"
        }

        task "motor1_sim" {
            driver = "raw_exec"

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " fcfDevsimMotor --port ${NOMAD_PORT_motor1} --cfg config/fcs/devsim/motor1.cfg.yaml --use-ext-ip"]
            }
        }
    }
}
