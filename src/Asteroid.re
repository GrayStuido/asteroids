let asteroidVelocity = 1.;

type sizeType =
  | Large
  | Medium
  | Small;

type axis =
  | XAxis
  | YAxis;

type t = {
  position: Vec.t,
  edges: list(Vec.t),
  sizeType,
  pixelSize: float,
  velocity: Vec.t,
  edgeModifier: float,
  radius: float,
};

let edgeCount = 7;

let sizeTypeProps = sizeType =>
  switch (sizeType) {
  | Large => (80., (-20.), 80.)
  | Medium => (50., (-10.), 50.)
  | Small => (30., (-5.), 30.)
  };

let edgeThing = 2. *. Js.Math._PI /. 7.;

let makeEdge = (size, num, edgeModifier) => {
  let modifier = num mod 2 == 1 ? Js.Math.random() *. edgeModifier : 0.;

  Vec.make(
    modifier +. size *. Js.Math.cos(float_of_int(num) *. edgeThing),
    modifier +. size *. Js.Math.sin(float_of_int(num) *. edgeThing),
  );
};

let rec makeEdges = (size, numRemaining, edgeModifier, edges) =>
  if (numRemaining > 0) {
    let edge = makeEdge(size, numRemaining, edgeModifier);

    makeEdges(size, numRemaining - 1, edgeModifier, [edge, ...edges]);
  } else {
    edges;
  };

let randomPosition = ((width, height)) => {
  let positionBase = Js.Math.random() > 0.5 ? XAxis : YAxis;

  switch (positionBase) {
  | XAxis => Vec.make(Math.random(0., width), 0.)
  | YAxis => Vec.make(0., Math.random(0., height))
  };
};

let make = (sizeType, screenSize) => {
  let (pixelSize, edgeModifier, radius) = sizeTypeProps(sizeType);
  {
    edges: makeEdges(pixelSize, edgeCount, edgeModifier, []),
    position: randomPosition(screenSize),
    pixelSize,
    edgeModifier,
    sizeType,
    radius,
    velocity:
      Vec.angle(
        Vec.make(asteroidVelocity, asteroidVelocity),
        Math.degreesToRadians(Js.Math.random() *. 360.),
      ),
  };
};

let update = (screenSize, asteroid) => {
  ...asteroid,
  position:
    Vec.add(asteroid.position, asteroid.velocity)
    |> Utils.normalizePosition(screenSize),
};

let draw = (ctx, {position, edges, pixelSize}) => {
  let {x, y}: Vec.t = position;

  Canvas.beginPath(ctx);
  Canvas.moveTo(
    ctx,
    x +. pixelSize *. Js.Math.cos(0.),
    y +. pixelSize *. Js.Math.sin(0.),
  );

  List.iter(
    (edge: Vec.t) => Canvas.lineTo(ctx, x +. edge.x, y +. edge.y),
    edges,
  );

  Canvas.closePath(ctx);
  Canvas.stroke(ctx);
};