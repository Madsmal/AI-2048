function AITile(position, value){
  Tile.apply(this,arguments);
  //this.name = name || 'Tom';
}
AITile.prototype = deepClone(Tile.prototype) ;


AITile.prototype.clone = function() {
  newTile = new AITile({ x: this.x, y: this.y }, this.value);
  return newTile;
}