import sys
import json
import ifw.fcf.clib.setup_command as sc
import ifw.fcf.clib.setup_buffer as sb
import ifw.fcf.clib.json_procs as json_obj

from ifw.fcf.clib import log
import tdm.fcs.clib.mos_setup as ss


sys.path.insert(0, "..")


class SetupBuffer(sb.SetupBuffer):
    """ Class manage the buffer of the setup request"""

    MOS_SCHEMA = r"""
    {
          "type": "object",
          "properties": {
            "action": {
              "type": "string",
              "enum": ["MOVE" ],
              "description": "Mos action."
            },
            "tip": {
              "type": "number",
              "description": "tip."
            },
            "tilt": {
              "type": "number",
              "description": "tilt."
            },
            "piston": {
              "type": "number",
              "description": "piston."
            }
          },
          "required": ["action","tip","tilt"]
        }
    """

    def __init__(self, cii):
        self._client = cii
        self._jsondata = None
        self._setup = sc.SetupCommand()

        """ Load basic schema for all standard devices """
        self._schema = json.loads(json_obj.SETUP_SCHEMA)

        """ Change the schema to add new device """
        """ add the new definition """
        self._schema['definitions']['mos'] = json_obj.load_json_string(SetupBuffer.MOS_SCHEMA)
        """ add new element the array """
        self._schema['definitions']['param']['oneOf'].append(json_obj.load_json_string('{"required": [ "mos"]}'))
        self._schema['definitions']['param']['properties']['mos'] = json_obj.load_json_string('{"$ref": "#/definitions/mos"}')

    def load_json_schema(self, json_schema):
        self._schema = json_obj.load_schema_file(json_schema)

    def json2object(self):
        """Loads and parse a JSON dictionary"""      
        super().json2object()
        for element in self._jsondata:
            buffer  = element['param']
            id = element['id']
            log.debug("element: %s", element)
            log.debug("id: %s", element['id'])
            log.debug("param: %s", element['param'])
            for type in element['param']:
                if type == "mos":
                    mos = ss.MosSetup(self._client.get_mal(), element['id'])
                    mos.set_dictionary(buffer[type])
                    self._setup.add_device(mos.get_buffer())
