open Opium.Std;

let host = Array.length(Sys.argv) >= 2 ? Sys.argv[1] : "localhost";
let port = 6379;

let conn_promise = Redis_lwt.Client.connect({host, port});

let (let.lwt) = Lwt.bind;
let (let.lwt_map) = (v, f) => Lwt.map(f, v);

let not_found = Rock.Response.make(~status=`Not_found, ());

let get_key =
  get("/:key", req => {
    let key = param(req, "key");
    let.lwt conn = conn_promise;
    let.lwt_map response = Redis_lwt.Client.get(conn, key);
    switch (response) {
    | Some(value) => Response.of_string(key ++ ": " ++ value)
    | None => not_found
    };
  });

let set_key =
  put("/:key", req => {
    let key = param(req, "key");
    let.lwt value = App.string_of_body_exn(req);
    let.lwt conn = conn_promise;
    let.lwt _success = Redis_lwt.Client.set(conn, key, value);
    Response.of_string("SET " ++ key ++ " " ++ value) |> Lwt.return;
  });
let delete_key =
  delete("/:key", req => {
    let key = param(req, "key");
    let.lwt _value = App.string_of_body_exn(req);
    let.lwt conn = conn_promise;
    let.lwt _success = Redis_lwt.Client.del(conn, [key]);
    Response.of_string("DEL " ++ key) |> Lwt.return;
  });

App.empty
|> App.port(3000)
|> get_key
|> set_key
|> delete_key
|> App.run_command;
