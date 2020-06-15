open Opium.Std;

let host = Array.length(Sys.argv) >= 2 ? Sys.argv[1] : "localhost";
let port = 6379;

let conn_promise = Redis_lwt.Client.connect({host, port});

let (let.lwt) = Lwt.bind;

let get_key =
  get("/:key", req => {
    let key = param(req, "key");
    let.lwt conn = conn_promise;
    let.lwt response = Redis_lwt.Client.get(conn, key);
    switch (response) {
    | Some(value) => `String(key ++ ": " ++ value) |> respond'
    | None => Rock.Handler.not_found(req)
    };
  });
let set_key =
  put("/:key", req => {
    let key = param(req, "key");
    let.lwt value = App.string_of_body_exn(req);
    let.lwt conn = conn_promise;
    let.lwt _success = Redis_lwt.Client.set(conn, key, value);
    `String("SET " ++ key ++ " " ++ value) |> respond';
  });
let delete_key =
  delete("/:key", req => {
    let key = param(req, "key");
    let.lwt _value = App.string_of_body_exn(req);
    let.lwt conn = conn_promise;
    let.lwt _success = Redis_lwt.Client.del(conn, [key]);
    `String("DEL " ++ key) |> respond';
  });
let () = {
  App.empty
  |> App.port(3000)
  |> get_key
  |> set_key
  |> delete_key
  |> App.start
  |> Lwt_main.run;
};
