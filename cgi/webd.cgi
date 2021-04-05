worker_processes  1;
pid /var/run/nginx.pid;

events {
    worker_connections  1024;
}


http {
    include       mime.types;
    default_type  application/octet-stream;

    sendfile        on;

    keepalive_timeout  65;

    upstream webd_backend {
        server localhost:19981;
        keepalive 32;
    }

    server {
        listen       80;
        server_name  localhost;

        location / {
            root   /etc/nginx/html/;
            index  index.html index.htm;

            fastcgi_keep_conn   on;
            fastcgi_pass        webd_backend;

            fastcgi_param  REQUEST_URI        $request_uri;
            fastcgi_param  QUERY_STRING       $query_string;
            fastcgi_param  SCRIPT_NAME        $fastcgi_script_name;
            fastcgi_param  REQUEST_METHOD     $request_method;

            fastcgi_param  CONTENT_TYPE       $content_type;
            fastcgi_param  CONTENT_LENGTH     $content_length;

            fastcgi_param  DOCUMENT_URI       $document_uri;
            fastcgi_param  DOCUMENT_ROOT      $document_root;

            fastcgi_param  SERVER_PROTOCOL    $server_protocol;
            fastcgi_param  HTTPS              $https if_not_empty;

            fastcgi_param  REMOTE_ADDR        $remote_addr;
            fastcgi_param  REMOTE_PORT        $remote_port;
            fastcgi_param  SERVER_ADDR        $server_addr;
            fastcgi_param  SERVER_PORT        $server_port;
            fastcgi_param  SERVER_NAME        $server_name;
        }

        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
    }
}