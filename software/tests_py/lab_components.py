import pytest
import logging
import time

from labgrid.target import Target
from pymeasure.instruments.siglenttechnologies import SDM3065XSC

from beeconlab.driver.custom_network_power_driver import CustomNetworkPowerDriver
from beeconlab.driver.scpi_driver import ScpiDriver
from beeconlab.driver.pymeasure_visa_adapter_driver import PymeasureVISAAdapterDriver

logger = logging.getLogger(__name__)

@pytest.fixture(scope="session")
def pdu_multimeter(target: Target):
    return target.get_driver(CustomNetworkPowerDriver, name="pdu_multimeter")

@pytest.fixture(scope="session")
def multimeter_visa(target: Target, pdu_multimeter: CustomNetworkPowerDriver):
    if not pdu_multimeter.get():
        pdu_multimeter.on()
        logger.info("Waiting for multimeter to boot")
        time.sleep(15)
    multimeter = target.get_driver(ScpiDriver, name="driver_multimeter_visa")
    yield multimeter.inst

@pytest.fixture(scope="session")
def multimeter_pymeasure(target: Target, pdu_multimeter: CustomNetworkPowerDriver):
    if not pdu_multimeter.get():
        pdu_multimeter.on()
        logger.info("Waiting for multimeter to boot")
        time.sleep(15)  # SDM3065X Bootzeit
    adapter = target.get_driver(PymeasureVISAAdapterDriver, name="driver_multimeter_pymeasure")
    return SDM3065XSC(adapter.adapter)