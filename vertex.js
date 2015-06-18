/*****************************************************************************

* Author : Ludvig Sundström

* File Name : vertex.js

* Purpose : Describes a vertex 

* Creation Date : 17-06-2015

* Last Modified : 

*****************************************************************************/

window.APP = window.APP || {};

APP.vertex = function (options) {
  
  var center      = options.center || APP.vector2D(0, 0);
  var force       = options.force || APP.vector2D(0, 0); 

  var that = {};
  that.shape      = options.shape || 'circle';
  that.dimensions = options.dimensions || 1;
  that.color      = options.color || 'grey';
  that.bonds      = options.bonds || [];

  that.attachBond = function (bond) {
    that.bonds.push(bond);  
  }

  that.setForce = function (vec) {
    force = vec;
  }
  
  that.addForce = function (vec) {
    force = APP.vector2D(force.x + vec.x, force.y + vec.y);
  }

  that.move = function() {
    var xprime = center.x + force.x;
    var yprime = center.y + force.y; 
    center = APP.vector2D(xprime, yprime);
  }
  
  that.zeroForce = function() {
    force.x = 0;
    force.y = 0;
  }

  that.getCenter = function () { return center };

  APP.vertices.push(that); 
  return that;

}
