#ifndef RECCALORIMETER_CREATEODDCELLS_H
#define RECCALORIMETER_CREATEODDCELLS_H

// Gaudi
#include "GaudiKernel/Service.h"
#include "k4Interface/ICaloCreateMap.h"

#include "TGeoManager.h"
#include "DDSegmentation/CartesianGridXY.h"
#include "DDSegmentation/CartesianGridXZ.h"

class IGeoSvc;

/** @class CreateODDCellss
 *
 *  Service constructing a list of cells: positions and sizes for the Open Data Detector.
 *
 *  @author Anna Zaborowska
 */

class CreateODDCells : public extends1<Service, ICaloCreateMap> {
public:
  /// Standard constructor
  explicit CreateODDCells(const std::string& aName, ISvcLocator* aSL);
  /// Standard destructor
  virtual ~CreateODDCells();
  /**  Initialize the map creator service.
   *   @return status code
   */
  virtual StatusCode initialize() final;
  /**  Finalize the map creator service.
   *   @return status code
   */
  virtual StatusCode finalize() final;

private:
  /// Pointer to the geometry service
  SmartIF<IGeoSvc> m_geoSvc;

  /// Names of the detector readout for volumes
  Gaudi::Property<std::vector<std::string>> m_readoutNames{this, "readoutNames", {"ECalBarrelCollection", "ECalEndcapCollection", "ECalEndcapCollection"}};
  /// Name of the volumes describing the top volume
  Gaudi::Property<std::vector<std::string>> m_topVolumeNames{this, "topVolumeNames", {"ECalBarrel", "ECalEndcap_endcap_0", "ECalEndcap_endcap_1"}};
  /// Readout descriptor of the volumes
  Gaudi::Property<std::vector<unsigned long long>> m_topVolumeIdentifiers{this, "topVolumeIdentifiers", {16,273,529}}; // 16<<0, (17<<0)+(1<<8), (17<<0)+(2<<8)
  /// Names of the hierarchy of active volumes in geometry separated by ":" (e.g. layer)
  Gaudi::Property<std::vector<std::string>> m_activeVolumeNames{this, "activeVolumeNames", {"stave_inner:layer:slice4", "stave_inner:layer:slice4", "stave_inner:layer:slice4"}};
  /// If the volume is a barrel: to store in ROOT file
  Gaudi::Property<std::vector<bool>> m_isBarrel{this, "isBarrel", {true, false, false}};
  /// If the volume is described in eta-cylindrical coordinates ( = r, phi, eta): to store in ROOT file
  Gaudi::Property<std::vector<bool>> m_isCylindrical{this, "isCylindrical", {false, false, false}};
  /// If the volume is described in eta-cylindrical coordinates ( = r, phi, z): to store in ROOT file
  Gaudi::Property<std::vector<bool>> m_isECCylindricalRPhiZ{this, "isECCylindricalRPhiZ", {true, true, true}};
  /// If the volume is described in Cartesian coordinates ( = x, y, z): to store in ROOT file
  Gaudi::Property<std::vector<bool>> m_isCartesian{this, "isCartesian", {false, false, false}};
  /// Name of output file
  std::string m_outputFileName;
};

std::array<double,5> trapezoidDimensions(uint64_t aVolumeId);
std::array<uint, 2> numberOfCells(uint64_t aVolumeId, const dd4hep::DDSegmentation::CartesianGridXY& aSeg);
std::vector<uint> numberOfCells(uint64_t aVolumeId, const dd4hep::DDSegmentation::CartesianGridXZ& aSeg);
unsigned int countPlacedVolumes(TGeoVolume* aHighestVolume, const std::string& aMatchName);
TGeoVolume* findVolume(TGeoVolume* aHighestVolume, const std::string& aTopVolName);

#endif /* RECALORIMETER_CREATEODDCELLS_H */
