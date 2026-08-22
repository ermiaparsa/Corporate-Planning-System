#include "corporate_planning/core/Assets.hpp"

using namespace std;

namespace corporate_planning::core {

Assets::Assets()
: cashOnHand(0.0),
cashInBank(0.0),
accountsReceivable(0.0),
land(0.0),
buildings(0.0),
machinery(0.0),
furniture(0.0),
vehicles(0.0),
installations(0.0),
rawMaterials(0.0),
workInProcess(0.0),
finishedGoods(0.0),
spareParts(0.0),
saleableWaste(0.0),
longTermInvestments(0.0),
externalInvestments(0.0) {
}

double Assets::calculateInventoryTotal() const {
return rawMaterials
+ workInProcess
+ finishedGoods
+ spareParts
+ saleableWaste;
}

double Assets::calculateTangibleAssetsTotal() const {
return land
+ buildings
+ machinery
+ furniture
+ vehicles
+ installations;
}

double Assets::calculateTotal() const {
return cashOnHand
+ cashInBank
+ accountsReceivable
+ calculateInventoryTotal()
+ calculateTangibleAssetsTotal()
+ longTermInvestments
+ externalInvestments;
}

void Assets::setCashOnHand(double value) {
cashOnHand = value;
}

void Assets::setCashInBank(double value) {
cashInBank = value;
}

void Assets::setAccountsReceivable(double value) {
accountsReceivable = value;
}

void Assets::setLand(double value) {
land = value;
}

void Assets::setBuildings(double value) {
buildings = value;
}

void Assets::setMachinery(double value) {
machinery = value;
}

void Assets::setFurniture(double value) {
furniture = value;
}

void Assets::setVehicles(double value) {
vehicles = value;
}

void Assets::setInstallations(double value) {
installations = value;
}

void Assets::setRawMaterials(double value) {
rawMaterials = value;
}

void Assets::setWorkInProcess(double value) {
workInProcess = value;
}

void Assets::setFinishedGoods(double value) {
finishedGoods = value;
}

void Assets::setSpareParts(double value) {
spareParts = value;
}

void Assets::setSaleableWaste(double value) {
saleableWaste = value;
}

void Assets::setLongTermInvestments(double value) {
longTermInvestments = value;
}

void Assets::setExternalInvestments(double value) {
externalInvestments = value;
}

double Assets::getCashOnHand() const {
return cashOnHand;
}

double Assets::getCashInBank() const {
return cashInBank;
}

double Assets::getAccountsReceivable() const {
return accountsReceivable;
}

} // namespace corporate_planning::core
