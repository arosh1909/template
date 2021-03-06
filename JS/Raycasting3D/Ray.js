class Ray {
  constructor(pos, angle) {
    this.pos = createVector(pos.x, pos.y);
    this.angle = angle;
    this.dir = createVector(1, 0);
    this.dir.rotate(angle);
  }

  setDir(x, y) {
    this.dir.x = x - this.pos.x;
    this.dir.y = y - this.pos.y;
    this.dir.normalize();
  }

  shiftAngle(delta) {
    this.angle -= delta;
    this.dir.rotate(delta);
  }

  intersect(border) {
    // Line line intersection: https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection

    let x1 = border.begin.x;
    let y1 = border.begin.y;

    let x2 = border.end.x;
    let y2 = border.end.y;

    let x3 = this.pos.x;
    let y3 = this.pos.y;

    let x4 = this.pos.x + this.dir.x;
    let y4 = this.pos.y + this.dir.y;

    let den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (den == 0 || -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den < 0)
      return;

    let t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;

    if (t > 0 && t < 1) {
      return new Point(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
    }

    return;
  }

  show() {
    fill(255);
    push();
    translate(this.pos.x, this.pos.y);
    stroke(255);
    ellipse(0, 0, 15, 15);
    // line(0, 0, this.dir.x * 20, this.dir.y * 20);
    pop();
  }
}
