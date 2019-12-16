type t('e, 'v);

type cancel =
  | NoCancel
  | Cancel(unit => unit);

type computation('e, 'v) = ('e => unit, 'v => unit) => cancel;

type cancelJs = option((. unit) => unit);

let safeCancel: cancelJs => unit =
  fun
  | Some(c) => c(.)
  | None => ();

type nodeback('e, 'v) = (Js.nullable('e), 'v) => unit;

/**
 * Creating
 */
[@bs.module "fluture"]
external make_: (('e => unit, 'v => unit) => cancelJs) => t('e, 'v) =
  "default";

let wrapComputation = (compute: computation('e, 'v), rej, res): cancelJs =>
  switch (compute(rej, res)) {
  | Cancel(c) => Some(((.) => c()))
  | NoCancel => None
  };

let make = (compute: computation('e, 'v)) =>
  make_(wrapComputation(compute));

[@bs.module "fluture"] external resolve: 'v => t('e, 'v) = "of";

[@bs.module "fluture"] external reject: 'e => t('e, 'v) = "reject";

[@bs.module "fluture"] external after: (int, 'v) => t('e, 'v) = "after";

[@bs.module "fluture"] external rejectAfter: (int, 'e) => t('e, 'v) = "rejectAfter";

[@bs.module "fluture"]
external attempt: (unit => 'v) => t(Js.Exn.t, 'v) = "try";

[@bs.module "fluture"]
external tryP: (unit => Js.Promise.t('v)) => t('e, 'v) = "tryP";

[@bs.module "fluture"]
external node: (nodeback('e, 'v) => unit) => t('e, 'v) = "node";

[@bs.module "fluture"]
external encase: ('a => 'v, 'a) => t(Js.Exn.t, 'v) = "encase";

[@bs.module "fluture"]
external encase2: (('a, 'b) => 'v, 'a, 'b) => t(Js.Exn.t, 'v) = "encase2";

[@bs.module "fluture"]
external encase3: (('a, 'b, 'c) => 'v, 'a, 'b, 'c) => t(Js.Exn.t, 'v) = "encase3";

[@bs.module "fluture"]
external encaseP: ('a => Js.Promise.t('v), 'a) => t(Js.Exn.t, 'v) = "encaseP";

[@bs.module "fluture"]
external encaseP2: (('a, 'b) => Js.Promise.t('v), 'a, 'b) => t(Js.Exn.t, 'v) =
  "encaseP2";

[@bs.module "fluture"]
external encaseP3:
  (('a, 'b, 'c) => Js.Promise.t('v), 'a, 'b, 'c) => t(Js.Exn.t, 'v) =
  "encaseP3";

[@bs.module "fluture"]
external encaseN: (('a, nodeback('e, 'v)) => unit, 'a) => t('e, 'v) = "encaseN";

[@bs.module "fluture"]
external encaseN2: (('a, 'b, nodeback('e, 'v)) => unit, 'a, 'b) => t('e, 'v) =
  "encaseN2";

[@bs.module "fluture"]
external encaseN3:
  (('a, 'b, 'c, nodeback('e, 'v)) => unit, 'a, 'b, 'c) => t('e, 'v) =
  "encaseN3";

/**
 * Transforming
 */
[@bs.module "fluture"]
external map: ('a => 'b, t('e, 'a)) => t('e, 'b) = "map";

[@bs.module "fluture"]
external mapRej: ('e => 'f, t('e, 'v)) => t('f, 'v) = "mapRej";

[@bs.module "fluture"]
external bimap: ('e => 'f, 'a => 'b, t('e, 'a)) => t('f, 'b) = "bimap";

[@bs.module "fluture"]
external chain: ('a => t('e, 'b), t('e, 'a)) => t('e, 'b) = "chain";

[@bs.module "fluture"]
external chainRej: ('e => t('f, 'v), t('e, 'v)) => t('f, 'v) = "chainRej";

[@bs.module "fluture"] external swap: t('e, 'v) => t('v, 'e) = "swap";

[@bs.module "fluture"]
external fold: ('e => 'a, 'v => 'a, t('e, 'v)) => t('f, 'a) = "fold";

/**
 * Consuming
 */
[@bs.module "fluture"]
external fork: ('e => unit, 'v => unit, t('e, 'v)) => cancelJs = "fork";

[@bs.module "fluture"]
external forkCatch:
  (Js.Exn.t => unit, 'e => unit, 'v => unit, t('e, 'v)) => cancelJs =
  "forkCatch";

[@bs.module "fluture"]
external value: ('v => unit, t('e, 'v)) => cancelJs = "value";

[@bs.module "fluture"]
external done_: (nodeback('e, 'v), t('e, 'v)) => cancelJs = "done";

[@bs.module "fluture"] external promise: t('e, 'v) => Js.Promise.t('v) = "promise";

/**
 * Parallelism
 */
[@bs.module "fluture"]
external race: (t('e, 'v), t('e, 'v)) => t('e, 'v) = "race";

[@bs.module "fluture"]
external both: (t('e, 'a), t('e, 'b)) => t('e, ('a, 'b)) = "both";

[@bs.module "fluture"]
external parallel: (int, array(t('e, 'v))) => t('e, array('v)) = "parallel";

/**
 * Utility
 */ [@bs.module "fluture"] external isFuture: 'a => bool = "isFuture";

[@bs.module "fluture"] external never: unit => t('e, 'v) = "never";

[@bs.module "fluture"] external isNever: 'a => bool = "isNever";

let let_ = (future, continuation) => future |> chain(continuation);
let and_ = both;
let try_ = (future, handler) => future |> chainRej(handler);
