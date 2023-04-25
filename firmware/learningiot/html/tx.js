class Tx {
    constructor() {
      this.sending = 200;
      this.txQueue = new Queue();
      window.setInterval(() => {
        this.processQueue()
      },1);
    }

    sendData(payload)
    {
        var data = new FormData();
        data.append( "json", JSON.stringify( payload ) );
        this.txQueue.enqueue(data);
    }

    processQueue()
    {
        var self = this;
        if (self.sending === 0)
        {
            if (self.txQueue.length > 0)
            {
                var data = self.txQueue.dequeue();
                self.sending = 200;
                fetch("/api/status",
                {
                    method: "POST",
                    body: data
                })
                .then(function(res){ self.sending = 2 })
                .then(function(data){ self.sending = 2 }) //alert( JSON.stringify( data ) )
            }
        } else 
        {
            if (self.sending > 0) {
                self.sending-=1;
            }
        }
    }
  }


if (typeof module != "undefined")
{
    module.exports.default = module.exports = {Tx}
}