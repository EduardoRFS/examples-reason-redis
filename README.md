# reason-redis

That project is used just as a pseudo "real project", it's a http server talking to a redis server

**Contains the following libraries and executables:**

```
reason-redis@0.0.0
│
bin/
    name:    ReasonRedisApp.exe
    main:    ReasonRedisApp
    require: reason-redis.lib
```

## Developing:

```
npm install -g esy
git clone <this-repo>
esy install
esy build
```

## Running Binary:

After building the project, you can run the main binary that is produced.

```
esy x ReasonRedisApp.exe
```

## Running Tests:

```
# Runs the "test" command in `package.json`.
esy test
```
