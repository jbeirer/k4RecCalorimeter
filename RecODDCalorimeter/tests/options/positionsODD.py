from Gaudi.Configuration import *
import os

from Configurables import ApplicationMgr, k4DataSvc, PodioOutput

# ECAL readouts
ecalBarrelReadoutName = "ECalBarrelCollection"
# Number of events
num_events = 1

podioevent = k4DataSvc("EventDataSvc")

from Configurables import GeoSvc
geoservice = GeoSvc("GeoSvc")
# if FCC_DETECTORS is empty, this should use relative path to working directory
path_to_detectors = os.environ.get("FCCcore", "")
detectors = [
        '../OpenDataDetector/xml/OpenDataDetector.xml'
]
# prefix all xmls with path_to_detectors
for det in detectors:
    geoservice.detectors += [os.path.join(path_to_detectors, det)]
geoservice.OutputLevel = WARNING

from Configurables import CreateODDCells

cells = CreateODDCells("cellsODD",
    outputFileName="ODD_cells.root",
    # Add both ECal and HCal readouts
    readoutNames=[
        "ECalBarrelCollection", "ECalEndcapCollection", "ECalEndcapCollection",
        "HCalBarrelCollection", "HCalEndcapCollection", "HCalEndcapCollection"
    ],
    topVolumeNames=[
        "ECalBarrel", "ECalEndcap_endcap_0", "ECalEndcap_endcap_1",
        "HCalBarrel", "HCalEndcap_endcap_0", "HCalEndcap_endcap_1"
    ],
    # ID encoding from detector description XML:
    # - System IDs: ECal barrel=16, ECal endcap=17, HCal barrel=19, HCal endcap=20
    # - The barrel field (3 bits) is encoded by shifting values by 8 bits (<<8)
    # - For barrel: barrel=0, for endcaps: endcap_0 has barrel=1, endcap_1 has barrel=2
    # See xml/OpenDataDetectorIdentifiers.xml
    topVolumeIdentifiers=[
        16,                  # ECal barrel: system=16, barrel=0
        17+(1<<8),           # ECal endcap_0: system=17, barrel=1
        17+(2<<8),           # ECal endcap_1: system=17, barrel=2
        19,                  # HCal barrel: system=19, barrel=0
        20+(1<<8),           # HCal endcap_0: system=20, barrel=1
        20+(2<<8)            # HCal endcap_1: system=20, barrel=2
    ],
    # Slices marked "sensitive" in XML are:
    # ECal: stave_inner:layer:slice4
    # HCal: stave_inner:layer:slice2 (Polystyrene is the third slice, index starts at 0)
    activeVolumeNames=[
        "stave_inner:layer:slice4", "stave_inner:layer:slice4", "stave_inner:layer:slice4",
        "stave_inner:layer:slice2", "stave_inner:layer:slice2", "stave_inner:layer:slice2"
    ],
    isBarrel=[
        True, False, False,
        True, False, False
    ],
    isXYZ=[
        False, False, False,
        False, False, False
    ],
    isEtaPhiR=[
        False, False, False,
        False, False, False
    ],
    isEtaPhiZ=[
        False, False, False,
        False, False, False
    ],
    isRPhiZ=[
        True, True, True,
        True, True, True
    ],
    OutputLevel=INFO
)


ApplicationMgr(
TopAlg = [     ],
    EvtSel = 'NONE',
    EvtMax = 1,
    ExtSvc = [geoservice, cells],
)
