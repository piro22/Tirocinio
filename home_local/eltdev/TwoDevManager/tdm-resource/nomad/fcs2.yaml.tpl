!cfg.include config/fcf/devmgr/definitions/server.yaml:

server: !cfg.type:FcfServer
    server_id       : 'fcs2'

    req_endpoint    : "zpb.rr://{{ range service "fcs2-req" }}{{ .Address }}:{{ .Port }}{{ end }}/"
    pub_endpoint    : "zpb.ps://{{ range service "fcs2-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/"

    db_timeout      : 2000
    scxml           : "config/fcf/devmgr/server/sm.xml"
    dictionaries    : ['dictionary/dit/stddid/primary.did.yaml', 'dictionary/fcf/devmgr/server/fcf.did']
    fits_prefix     : "INS2"
    oldb_prefix     : 'elt/prv'
    req_timeout     : 120000
    log_properties  : "config/fcs/server/log_properties.cfg"
    version         : '0.0.0'
    gui: {
          containers: [
           {
             name: 'Devices',
             position: 'LEFT'
           },
           {
             name: 'Sensors',
             position: 'RIGHT'
           }
         ]
       }
    devices         : [
    {
    name: 'lamp1',
    type: Lamp,
    cfgfile: "local/lamp1.yaml"
    }
    ]


