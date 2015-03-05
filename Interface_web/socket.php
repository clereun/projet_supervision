<?php

define('BR', "\n");
define('CLIENT_LOOP_SLEEP', 5000000);

abstract class ClientBase
{
    protected static $socket; # Socket du client
    protected static $address; # Adresse IP du client
    protected static $timeout = 1000; # Time out de connection au serveur

    protected function  __construct()
    {
        self::$address = $this->getIP();
    }

    protected function getIP()
    {
        if (!empty($_SERVER['HTTP_CLIENT_IP']))   //check ip from share internet
        {
            $ip = $_SERVER['HTTP_CLIENT_IP'];
        }
        else if (!empty($_SERVER['HTTP_X_FORWARDED_FOR']))   //to check ip is pass from proxy
        {
            $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
        }
        else
        {
            $ip = $_SERVER['REMOTE_ADDR'];
        }

        return $ip;
    }
    /*
     * Ecriture sur la socket client
     */
    protected function read($size = 1024)
    {
        return socket_read(self::$socket, $size);
    }
    /*
     * Lecture sur la socket client
     */
    protected function write($datas)
    {
        return socket_write(self::$socket, $datas, strlen($datas));
    }
    /*
     * Instruction live, teste si le serveur recoit bien le message
     */
    protected function live()
    {
        return $this->write('@live');
    }
    /*
     * Connection au serveur
     */
    public function connect($server, $port, $protocol = SOL_TCP)
    {
        if ($protocol != SOL_TCP && $protocol != SOL_UDP)
            throw new Exception(__CLASS__.'::'.__FUNCTION__.'::INVALID_PROTOCOL');

        if
        (
            !is_resource(self::$socket) &&
            !is_resource(self::$socket = socket_create(AF_INET, SOCK_STREAM, $protocol))
        )
            throw new Exception(__CLASS__.'::'.__FUNCTION__.'::UNABLE_TO_CREATE_SOCKET');

        socket_set_option(self::$socket, SOL_SOCKET, SO_RCVTIMEO, array('sec' => self::$timeout, 'usec' => 0));

        switch (socket_connect(self::$socket, $server, $port))
        {
            case true:
                socket_set_nonblock(self::$socket);
                
                return true;
            default:
                throw new Exception(__CLASS__.'::'.__FUNCTION__.'::CONNECT_ERROR::'.socket_strerror(socket_last_error()));
        }
    }
    /*
     * Fermeture de la socket client
     */
    public function  shutdown()
    {
        $this->continue = false;

        parent::shutdown();

        echo 'Shutdown client'.BR;
    }
    /*
     * A implementer dans la classe client
     */
    #abstract protected function loop(); # Fonction loop du client
    abstract protected function action(); # Fonction action du client
    abstract protected function timeout(); # Fonction timeout du client

}

class Client extends ClientBase
{
    private $startAt;
    private $continue = true;

    public function  __construct()
    {
        parent::__construct();
    }
    /*
     * Initialisation du client
     */
    public function init()
    {
        echo 'Connected to server !'.BR;
        
        $this->startAt = time();
    }
    /*
     * Boucle infinie du client, ici toutes les actions du client
     
    public function loop()
    {
        if ($this->timeout())
            $this->shutdown();

        echo 'Sending datas to server...'.BR;

        $this->write('Coucou ! :)');
        $this->listen();
        
        ob_flush();
        usleep(CLIENT_LOOP_SLEEP);

        if ($this->continue)
            $this->loop();
    }
    */


    /*
     * actions du client web
     */
     
    public function action()
    {
        if ($this->timeout())
            $this->shutdown();

        echo 'Sending datas to server...'.BR;

        $this->write('Connection établie depuis de client web !');
        $this->listen();
        
    }


    /*
     * Ecoute des messages serveur
     */
    protected function listen()
    {
        if (($datas = $this->read()))
        {
            echo 'Server response : '.$datas.BR;
        }
    }
    /*
     * Si le serveur timeout cette fonction sera appeler pour terminer la connection
     */
    protected function timeout()
    {
        if ($this->live() === false)
        {
            echo 'Server timeout. Connection time : '.round(time() - $this->startAt).' s @++ !'.BR;
            
            return true;
        }

        return false;
    }


}
/*
 * Lancement du client
 */
$cli = new Client();

try
{
    if ($cli->connect('127.0.1.1', 7002))
    {
        $cli->init();
        $cli->action();
        $cli->listen();
        if ( $cli->listen() == "Server response : ok") 
        {
           $this->write('message ok bien recu par le client !');
        }
    }
    else
    {
        echo 'Connection failed...';
    }
}
catch(Exception $e)
{
    echo $e->getMessage().BR;
}
?>
