/**
 * @author Peter Kelley
 * @author pgkelley4@gmail.com
 *
 * Modified by Ludvig Sundstrom 18/6-15
 */

/**
 * See if two line segments intersect. This uses the 
 * vector cross product approach described below:
 * http://stackoverflow.com/a/565282/786339
 * 
 * @param {Object} p point object with x and y coordinates
 *  representing the start of the 1st line.
 * @param {Object} p2 point object with x and y coordinates
 *  representing the end of the 1st line.
 * @param {Object} q point object with x and y coordinates
 *  representing the start of the 2nd line.
 * @param {Object} q2 point object with x and y coordinates
 *  representing the end of the 2nd line.
 */

document.addEventListener('DOMContentLoaded', function (e) {

  window.APP = window.APP || {};

  APP.intersectionPoint = function (p, p2, q, q2) {
    // t is for p
    // u is for q
    var result = null;

    var r = subtractPoints(p2, p);
    var s = subtractPoints(q2, q);

    var uNumerator = crossProduct(subtractPoints(q, p), r);
    var denominator = crossProduct(r, s);

    if (denominator == 0) {
      // lines are paralell
      return result;
    }

    var u = uNumerator / denominator;
    var t = crossProduct(subtractPoints(q, p), s) / denominator;

    if ((t > 0.1) && (t < 0.9) && (u > 0.1) && (u < 0.9)) {
      result = { t: t, u: u };
    }
    return result;
  };

  /**
   * Calculate the cross product of the two points.
   * 
   * @param {Object} point1 point object with x and y coordinates
   * @param {Object} point2 point object with x and y coordinates
   * 
   * @return the cross product result as a float
   */
  function crossProduct(point1, point2) {
    return point1.x * point2.y  - point1.y * point2.x;
  }

  /**
   * Subtract the second point from the first.
   * 
   * @param {Object} point1 point object with x and y coordinates
   * @param {Object} point2 point object with x and y coordinates
   * 
   * @return the subtraction result as a point object
   */ 
  function subtractPoints(point1, point2) {
    var result = {};
    result.x = point1.x - point2.x;
    result.y = point1.y - point2.y;

    return result;
  }

});