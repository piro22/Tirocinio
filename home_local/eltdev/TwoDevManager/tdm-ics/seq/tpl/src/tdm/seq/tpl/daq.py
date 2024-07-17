"""
"""
import datetime
import json
from dataclasses import dataclass
from enum import Enum
from numbers import Real
from typing import Any, Dict, Iterable, List, Optional, Union
from urllib.parse import urlparse

from elt import configng, pymal
from ModDaqif.Daqif import (AwaitDaqReply, DaqException, DaqReply, DaqState,
                            DaqSubState)
from ModDaqif.Daqif.OcmDaqControl import OcmDaqControlAsync
from seq.lib.log import getUserLogger

logger = getUserLogger()


@dataclass
class PrimaryDataSource:
    """Specifies a primary data source such as a detector to acquire data from"""

    name: str  # Name to associate with source
    rr_uri: str  # CII request/reply URI

    def to_json(self) -> Dict:
        """Serialize to JSON"""
        return dict(type="primarySource", sourceName=self.name, rrUri=self.rr_uri)


@dataclass
class MetadataSource:
    """Specifies a metadata source such as a device manager to acquire data from"""

    name: str  # Name to associate with source
    rr_uri: str  # CII request/reply URI

    def to_json(self) -> Dict:
        """Serialize to JSON"""
        return dict(type="metadataSource", sourceName=self.name, rrUri=self.rr_uri)


SourceTypes = Union[PrimaryDataSource, MetadataSource]


@dataclass
class OlasReceiver:
    """Specifies online archiver receiver.

    Corresponds to named JSON type `OlasReceiver`.
    """

    host: str
    path: Optional[str]

    def to_json(self) -> Dict:
        """Serialize to JSON"""
        spec = dict(type="olasReceiver", path=self.path)
        if self.host:
            spec["host"] = self.host
        return spec


def LoadOlasConfig(instrument_name: str) -> OlasReceiver:
    """Load olas configuration using instrument configuration"""
    doc = configng.CiiConfigClient.load(f"cii.config://local/config/{ instrument_name }.yaml")
    cfg = doc.get_instances()
    return OlasReceiver(
        path=cfg.olas.directory.get_value(),
        host=cfg.olas.host.get_value() if cfg.olas.has("host") else None,
    )


def AppendUriService(uri: str) -> str:
    """Append DAQ service to URI if not provided"""
    if not urlparse(uri).path:
        return f"{uri}/daq"
    return uri


class SimpleDaq:
    """Create and manage simple DataAcquisition which include instruction to archive resulting data
    product in OLAS using configuration"""

    def __init__(
        self,
        *,
        mal: pymal.CiiFactoryModule.CiiFactory,
        uri: str,
        sources: Iterable[SourceTypes],
        merge_target: Optional[str] = None,
        await_completion_interval: Optional[float] = None,
        olas: Optional[OlasReceiver] = None,
    ):
        """Initializes DAQ specification but does not start DAQ.

        :param mal: CII MAL Factory
        :param uri: MAL base or full uri e.g. zpb.rr://10.0.1.100:5041 or
        zpb.rr://10.0.1.100:5041/daq
        :param sources: List of sources
        :param merge_target: Optionally specifies source name for which the first FITS file produced
        becomes the target into all other FITS files are merged into.
        :param olas: Optional OLAS receiver configuration.
        :param await_completion_interval: This is the interval, in seconds, between requests sent
        to data sources to await the completion of the Data Acquisition. If this number exceed the
        MAL timeout it will stop working.
        """
        self._mal = mal
        self._uri = AppendUriService(uri)
        self._id = None
        self._default_timeout = 15
        self._await_completion_interval = (
            float(await_completion_interval) if await_completion_interval is not None else 4.0
        )
        self._spec: Dict[str, Any] = dict(
            sources=[s.to_json() for s in sources],
            awaitCompletionInterval=self._await_completion_interval,
        )
        if merge_target is not None:
            self._spec["mergeTarget"] = {"sourceName": merge_target}

        if olas is not None:
            self._spec["receivers"] = [olas.to_json()]

    @property
    def specification(self):
        """Return specification"""
        return self._spec

    @property
    def daq_id(self) -> str:
        """Return DAQ ID or throw RuntimeError if DAQ has not been started"""
        if self._id is None:
            raise RuntimeError("DAQ has not been started")
        return self._id

    def get_client(self, timeout=None):
        """Get MAL rr client with specified timeout"""
        td = datetime.timedelta(seconds=timeout or self._default_timeout)
        return self._mal.getClient(self._uri, OcmDaqControlAsync, qos=pymal.rr.qos.ReplyTime(td))

    async def async_start_daq(self) -> DaqReply:
        """Start DAQ using current value of specification."""
        spec = json.dumps(self.specification)
        logger.info("daq: StartDaqV2(): specification='%s'", spec)
        client = self.get_client()
        reply = await client.StartDaqV2(spec).create_future()
        self._id = reply.getId()
        return reply

    async def async_await_daq_state(
        self, *, state: DaqState, substate: DaqSubState, timeout: Real
    ) -> AwaitDaqReply:
        """Wait for specified state and substate"""
        client = self.get_client(timeout + 5)  # add 5s for request/reply overhead.
        return await client.AwaitDaqState(self.daq_id, state, substate, timeout).create_future()

    async def async_abort_daq(self) -> DaqReply:
        """Abort ongoing DAQ"""
        return await self.get_client().AbortDaq(self.daq_id).create_future()

    async def async_force_abort_daq(self) -> DaqReply:
        """Force abort ongoing DAQ"""
        return await self.get_client().ForceAbortDaq(self.daq_id).create_future()

    async def async_stop_daq(self) -> DaqReply:
        """Stop ongoing DAQ"""
        return await self.get_client().AbortDaq(self.daq_id).create_future()

    async def async_force_stop_daq(self) -> DaqReply:
        """Force stop ongoing DAQ"""
        return await self.get_client().ForceAbortDaq(self.daq_id).create_future()
