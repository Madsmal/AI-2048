var deepClone = function(source,target){
  source = source || {} ;
  target = target || {};
  var toStr = Object.prototype.toString ,
    arrStr = '[object array]' ;
  for(var i in source){
    if(source.hasOwnProperty(i)){
      var item = source[i] ;
      if(typeof item === 'object'){
        target[i] = (toStr.apply(item).toLowerCase() === arrStr) ? [] : {} ;
        deepClone(item,target[i]) ;
      }else{
        target[i] = item;
      }
    }
  }
  return target ;
} ;


