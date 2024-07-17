!cfg.include config/sup/subsim/server/definitions.yaml:

server: !cfg.type:SubSim
    server_id       : 'subsim1'

    req_endpoint    : "zpb.rr://{{ range service "subsim1-req" }}{{ .Address }}:{{ .Port }}{{ end }}/"
    pub_endpoint    : "zpb.ps://{{ range service "subsim1-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/"

    db_timeout      : 2000
    oldb_prefix     : 'elt/tdm'
    log_properties  : "config/utils/bat/log_properties.cfg"
    scxml           : "config/sup/subsim/server/sm.xml"
    commands      : [
    {
    name: Init,
    reply_ok: true,
    reply_delay: 3000,
    reply_error_msg: "Init failed - unknown reasons"
    },
    {
    name: Enable,
    reply_ok: true,
    reply_delay: 5000,
    reply_error_msg: "ERROR: Enable failed"
    },
    {
    name: Disable,
    reply_ok: true,
    reply_delay: 1000,
    reply_error_msg: "ERROR: Disable failed"
    },
    {
    name: Setup,
    reply_ok: true,
    reply_delay: 7000,
    reply_error_msg: "ERROR: Setup failed"
    }
    ]

