# LAB00 - Introduction to Docker

1. [TL;DR](#too-long-didnt-read)
2. [Deploy a PostgreSQL image](#deploy-a-postgresql-image)
	1. [Start the Docker deamon](#start-the-docker-deamon)
	2. [Start a PostgreSQL instance](#start-a-postgresql-instance)
3. [Destroy the PostgreSQL instance](#destroy-the-postgresql-instance)
4. [Deploy a PostgreSQL instance with volume](#deploy-a-postgresql-instance-with-volume)


## Too Long; Didn't Read

- Pull images from Docker Hub: `docker pull <name_of_the_image>`
- To create and run a container (without persistent volume): `docker run -p <port_host>:<port_docker> --name <name_of_the_container> -e <ENVIRONMENT_VARIABLE> -d <name_identifier>` 
- To create and run a container (with persistent volume):
	- create a Docker volume: `docker volume create <name_of_the_volume>`
	- run the container: `docker run -p <port_host>:<port_docker> -v <name_of_the_volume>:/path/to/host/data/folder --name <name_of_the_container> -e <ENVIRONMENT_VARIABLE> -d <name_identifier>` 

## Deploy a PostgreSQL image

In the first part of this laboratory, we are going to deploy a PostgreSQL image on docker, in order to test it and play with the databases. We will see that the database tables and entries we upload, will be preserved from one session to the other.

> We can see the PostgreSQL Docker documentation [here](https://github.com/docker-library/docs/blob/master/postgres/README.md). 

### Start the docker deamon

First, we need to actually start the **Docker deamon**.

When we use Docker, we are essentially using a client-server architecture. The Docker client communicates with the Docker daemon (also known as the **Docker engine**) to perform various tasks such as building, running, and managing Docker containers.

The Docker daemon is responsible for managing the container's lifecycle, networking, storage, and other core Docker services. Therefore, to use Docker, you need to start the Docker daemon so that the Docker client can communicate with it and perform Docker operations.

### Start a PostgreSQL instance

Once the Docker deamon has started, we can now interact with the "server" with the command-line client interface. For a more detailed list of commands that can be run with the command line, refer to the official documentation [here](https://docs.docker.com/engine/reference/commandline/cli/) or run the `help` tag as:

```bash
docker help
```

We can proceeds in creating a PostgreSQL container by running the command:

```bash
docker run --name my-postgres-container -e POSTGRES_PASSWORD=mysecretpassword -p 5432:5432 -d postgres
```
This command will download the latest PostgreSQL image from Docker Hub, create a new container named "my-postgres-container", set the password for the default "postgres" user, and map port 5432 on the host machine to port 5432 in the container.

> Note that the `-e` tag is used to defined the environment variables, `-p` is used to map the ports of the Docker environment to the local environment and `-d` is used to create a **detached** instance.

We can now connect to the PostgreSQL running container through a PostgreSQL client (e.g. **pgAdmin**, `psql` (the command-line client), **IntelliJ IDEA**, etc...), providing the following information:
- host: `localhost`
- port: `5432`
- password: `mysecretpassword`

Once connected, we can use the database as we like, running SQL statements as usual, creating tables and managing data.

## Destroy the PostgreSQL instance

To remove the PostgreSQL (running) instance, we can run the command: 
```bash
docker rm -f <name_of_the_container>
```

This command removes the container named `<name_of_the_container>` from the machine. The `-f` flag is used to force the removal of the container, even if it's currently running. When we run this command, Docker stops the container (if it is running) and removes it, including all data stored inside it.

> **Because we didn't use a Docker volume to persist the PostgreSQL data, all data stored inside the previous container is now lost. Indeed, recreating the container running the same command WILL NOT provide persistence of data.**

## Deploy a PostgreSQL instance with volume
Now, we will recreate the PostgreSQL container again, but this time we will add a Docker volume to persist the data between container restarts.

To do this, we will create a Docker volume using the following command:

```bash
docker volume create <name_of_the_volume>
```

This command will create a new Docker volume named `<name_of_the_volume>`. This volume will be used to persist the PostgreSQL data between container restarts.

Finally, we will recreate the PostgreSQL container, but this time we will use the `-v` flag to mount the `<name_of_the_volume>` volume to the `/path/to/host/data/folder` directory inside the container. This will allow us to persist the PostgreSQL data on our host machine.

We will use the following command to recreate the container:

```bash
docker run --name <name_of_the_container> -e POSTGRES_PASSWORD=mysecretpassword -p 5432:5432 -v <name_of_the_volume>:/path/to/host/data/folder -d postgres
```

This command will create a new PostgreSQL container named `<name_of_the_container>`, with the same configuration as the previous container. However, this time, we have added the `-v` flag to mount the `<name_of_the_volume>` volume to the container's `/path/to/host/data/folder` directory.

Now, when we create new databases or tables inside the PostgreSQL container, the data will be stored in the `<name_of_the_volume>` volume on our host machine. This means that even if we stop and remove the container, the data will be persisted and we can start a new container and access the data from the previous container.