// CDCH layer description
//
// Original author G. Tassielli
//

#ifndef Layer_BS_hh
#define Layer_BS_hh

#include <deque>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "DCHbsCell.hh"
#include "DCHbsFoil.hh"
#include "DCHbsLayerDetail.hh"
#include "DCHbsLayerId.hh"
#include "manageException.hh"

// CDCH includes

namespace tdch{
	class TDCHMaker;
}

namespace dchbs {

class Layer{

//  friend class SuperLayer;
////  friend class CDCHDevice;
//  friend class CDCHtracker;
//  friend class CDCHMaker;

	friend class tdch::TDCHMaker;

public:

  enum Ltype {undefined=-1, wire, gas, serv, foil};

  // A free function, returning void, that takes a const Cell& as an argument.
  typedef void (*CellFunction)( const Layer& s);

//  Layer();

  // Constructor using cell/wire/layer info.
  Layer( boost::shared_ptr<LayerId>& id,
         boost::shared_ptr<LayerDetail> &detail,
//         int detailIndex,
         std::vector<boost::shared_ptr<Cell> > &cells,
         std::vector<boost::shared_ptr<Wire> > &fieldWires
         );

  virtual ~Layer ();

  virtual const LayerId& Id() const { return *(_id.get());}

  virtual LayerDetail& getLDetail() const { return *(_detail.get());}

  boost::shared_ptr<LayerDetail> getDetail() const { return _detail;}

  /*const*/ Ltype getLayerType() const { return _layerType; }

  int nCells() const { return _nCells; }

  int nFieldWires() const { return _nFiledWires; }

  double voxelizationFactor() const { return _voxelizationFactor; }

  boost::shared_ptr<Cell> getCell( int n ) const /*throw(cet::exception)*/ {
        //if (n>=0 && n<_nCells) return _cells.at(n);
        //else {//throw cet::exception("GEOM")<< "Cell number: "<< n <<" not present in "<<_id;
        if (n<0 || n>=_nCells) {
          exc::exceptionG4 e("GEOM","Fatal Error",0 );
          e<< "Cell number: "<< n <<" not present in "<<_id;
          e.error();
        }
        return _cells.at(n);
  }

  boost::shared_ptr<Cell> getCell( const CellId& id ) const {
    return getCell(id.getCell());
  }

  boost::shared_ptr<Wire> getFWire( int n ) const /*throw(cet::exception)*/ {
        //if (n>=0 && n<_nFiledWires) return _fieldWires.at(n);
        //else { //throw cet::exception("GEOM")<< "Field wire number: "<< n <<" not present in "<<_id;
        if (n<0 || n>=_nFiledWires){
          exc::exceptionG4 e("GEOM","Fatal Error",0);
          e<< "Field wire number: "<< n <<" not present in "<<_id;
          e.error();
        }
        return _fieldWires.at(n);
  }

  boost::shared_ptr<Wire> getFWire( const WireId& id ) const {
    return getFWire(id.getWire());
  }

  boost::shared_ptr<Foil> getFFoil( int n ) const /*throw(cet::exception)*/ {
        //if (n>=0 && n<_nFiledWires) return _fieldWires.at(n);
        //else { //throw cet::exception("GEOM")<< "Field wire number: "<< n <<" not present in "<<_id;
        if (n<0 || n>=_nFiledFoils){
          exc::exceptionG4 e("GEOM","Fatal Error",0);
          e<< "Field Foil number: "<< n <<" not present in "<<_id;
          e.error();
        }
        return _fieldFoils.at(n);
  }

  boost::shared_ptr<Foil> getFFoil( const FoilId& id ) const {
    return getFFoil(id.getFoil());
  }

protected:

  Layer();

//  Layer( LayerId* id );
//
  Layer( LayerId* id,
         LayerDetail* detail );

  // Identifier
  boost::shared_ptr<LayerId> _id;

  // Detailed description of a layer.
  boost::shared_ptr<LayerDetail> _detail;
//  int _detailIndex;

  Ltype _layerType;
  int _nCells;
  int _nFiledWires;
  int _nFiledFoils;

  double _voxelizationFactor;

  // Pointers to the cells and field wires in this layer.
  std::vector<boost::shared_ptr<Cell> > _cells;
  std::vector<boost::shared_ptr<Wire> > _fieldWires;
  std::vector<boost::shared_ptr<Foil> > _fieldFoils;

  void addCell(Cell *cell){
          _cells.push_back(boost::shared_ptr<Cell>(cell));
          _nCells++;
  }

  void addFieldWire(Wire *tmpWire){
          _fieldWires.push_back(boost::shared_ptr<Wire> (tmpWire));
          _nFiledWires++;
  }

  void addFieldFoil(Foil *tmpFoil){
          _fieldFoils.push_back(boost::shared_ptr<Foil> (tmpFoil));
          _nFiledFoils++;
  }


};

}  //namespace dchbs

#endif /* Layer_BS_hh */
