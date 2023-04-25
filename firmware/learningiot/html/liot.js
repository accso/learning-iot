class LIotBoard {
    constructor(canvasid="LIoTCanvas",width=262,height=345) {
        let self = this;
        this.position = {
            led: {
                x: 70,
                y: 310,
                radius: 30,
            },
            button: {
                x: 180,
                y: 305,
                radius: 20,
            }
        }
        this.canvas = {
            handle:undefined,
            id:canvasid,
            width:width,
            height:height,
            offset:{
                x:0,
                y:0
            }
        };

        this.leds = {
            r: false,
            g: false,
            b: false
        }

        this.buttonPressed=false;

        this.valuesUpdated=false;

        this.updateInternally = false;

        this.onupdated = undefined;

        this.forceRedrawTriggered = false;

        this.reinitCanvas(this.canvas.id);

        this.backgroundLoaded=false;
        this.background = new Image();
        this.background.addEventListener("load", (e) => {
            this.backgroundLoaded=true;
        });
        this.background.src = "pin-list-small.png"

        window.setInterval(() => {
            if (this.redraw)
            {
                this.redraw = false;
                this.draw();
            }
        },200);

        window.addEventListener('resize', function() {
            self.updateVars()
        });

        window.addEventListener('scroll', function() {
            self.updateVars()
        });
    }

    /**
     * Update all variables which could change by a window resize
     */
    updateVars()
    {
        var self = this;
        self.canvas.offset.x = self.canvas.handle.getBoundingClientRect().left;
        self.canvas.offset.y = self.canvas.handle.getBoundingClientRect().top;
    }

    checkClickedInside(clickpos,obj)
    {
        var dx = Math.abs(clickpos.x - obj.x);
        var dy = Math.abs(clickpos.y - obj.y);
        var clickradius = Math.sqrt(dx*dx+dy*dy);
        if (clickradius < obj.radius)
        {
            return true;
        }
        return false;
    }
    
    updateCursor(self,event)
    {

    }

    clicked(self,event)
    {
        let clickPos = {
            x:event.clientX,
            y:event.clientY
        };

        let canvasClickPos = {
            x:(clickPos.x - self.canvas.offset.x),
            y:(clickPos.y - self.canvas.offset.y)
        };

        console.log("Pos-Clicked: " + canvasClickPos.x + ", " + canvasClickPos.y);

        if (self.checkClickedInside(canvasClickPos,self.position.button))
        {
            var response = {
                button: false
            }

            if (self.buttonPressed)
            {
                response.button = false;
            } else 
            {
                response.button = true;
            }

            if (self.fireOnUpdated !== undefined)
            {
                self.fireOnUpdated(response);
            }
        }
    }

    /**
     * Execute as soon class was loaded
     */
    classLoaded()
    {
        var self = this;
        this.updateVars();
        this.draw();

        this.canvas.handle.addEventListener('mousedown', function(event) {
            self.clicked(self,event);
        });

        this.canvas.handle.addEventListener('mousemove', function(event) {
            if(event.buttons == 1) {
               event.preventDefault();
               self.clicked(self,event);
            }
            self.updateCursor(self,event)
        });

        this.canvas.handle.addEventListener('touchstart', function(event) {
            if(event.touches.length == 1) {
                event.preventDefault();
                self.clicked(self,event.touches[0]);
            }
        });
        
        this.canvas.handle.addEventListener('touchmove', function(event) {
            if(event.touches.length == 1) {
                event.preventDefault();
                self.clicked(self,event.touches[0]);
            }
        });
    }

    convertBool(value)
    {
        if (value === "true")
        {
            return true;
        } else if (value === true)
        {
            return true;
        } else
        {
            return false;
        }
    }

    updateFromEdge()
    {
        var self = this;
        fetch('/api/status')
        .then(response => response.json())
        .then(response => {
            if (response.red !== undefined)
            {
                var newVal = self.convertBool(response.red);
                this.setLedRed(newVal,false);
            }
            if (response.green !== undefined)
            {
                var newVal = self.convertBool(response.green);
                this.setLedGreen(newVal,false);
            }
            if (response.blue !== undefined)
            {
                var newVal = self.convertBool(response.blue);
                this.setLedBlue(newVal,false);
            }
            if (response.button !== undefined)
            {
                var newVal = self.convertBool(response.button);
                this.setButton(newVal,false);
            }
        })
        .catch((error) => {
            
        });
    }

    /**
     * Check class is loaded...
     */
    checkLoaded()
    {
        let self = this;
        if (!this.backgroundLoaded)
        {
            return;
        }
        if(typeof this.canvas.handle == 'undefined' || self.canvas.handle == null)
        {
            self.canvas.handle = document.getElementById(self.canvas.id);
            if(typeof this.canvas.handle == 'undefined' || self.canvas.handle == null)
            {
                this.canvas.handle = undefined;
            } else
            {
                this.classLoaded();
            }
        }
        
    }

    /**
     * Create drawing context
     * 
     * @returns context
     */
    createDrawContext()
    {
        var ctx = this.canvas.handle.getContext("2d");
        return ctx;
    }

    /**
     * Reinitialize canvas
     * 
     * @param {*} canvasName new canvas name
     */
    reinitCanvas(canvasName)
    {
        let self = this;

        self.canvas.id = canvasName;
        self.canvas.handle = undefined;
        let loadingInterval = window.setInterval(() => {
            self.checkLoaded();
            if(typeof self.canvas.handle !== 'undefined' && self.canvas.handle !== null)
            {
                window.clearInterval(loadingInterval);
                console.log("Canvas Initialized");
            }
        },500);
    }

    forceUpdateCanvas()
    {
        var self = this;
        if (!this.forceRedrawTriggered)
        {
            this.forceRedrawTriggered = true;
            window.setTimeout(() => {
                self.forceRedrawTriggered = false;
                self.draw();
            },100);
        }
    }

    fireOnUpdated(response)
    {
        var self = this;
        if ((self.onupdated !== undefined)  && (self.onupdated !== null) && (typeof self.onupdated !== undefined))
        {
            this.onupdated(response);
        }
    }

    setButton(newValue,fireUpdate=true)
    {
        var response = {
            button: newValue
        }
        var oldVal = this.buttonPressed;
        this.buttonPressed = newValue;
        this.valuesUpdated=true;
        this.forceUpdateCanvas();
        if ((oldVal != newValue) && (fireUpdate) && (this.fireOnUpdated !== undefined))
        {
            this.fireOnUpdated(response);
        }
    }

    setLeds(r=false,g=false,b=false,fireUpdate=true)
    {
        var response = {
            red:r,
            green:g,
            blue:b
        }
        var oldVal = {
            r:this.leds.r,
            g:this.leds.g,
            b:this.leds.b,
        }
        this.leds.r = r;
        this.leds.g = g;
        this.leds.b = b;
        this.valuesUpdated=true;
        this.forceUpdateCanvas();
        if (((r != oldVal.r) ||  (g != oldVal.g) ||  (b != oldVal.b)) && (fireUpdate) && (this.fireOnUpdated !== undefined))
        {
            this.fireOnUpdated(response);
        }
    }

    setLedRed(newValue,fireUpdate=true)
    {
        var response = {
            red:newValue,
        }
        var oldVal = this.leds.r;
        this.leds.r = newValue;
        this.valuesUpdated=true;
        this.forceUpdateCanvas();
        if ((oldVal != newValue) && (fireUpdate) && (this.fireOnUpdated !== undefined))
        {
            this.onupdated(response);
        }
    }

    setLedGreen(newValue,fireUpdate=true)
    {
        var response = {
            green:newValue,
        }
        var oldVal = this.leds.g;
        this.leds.g = newValue;
        this.valuesUpdated=true;
        this.forceUpdateCanvas();
        if ((oldVal != newValue) && (fireUpdate) && (this.fireOnUpdated !== undefined))
        {
            this.onupdated(response);
        }
    }

    setLedBlue(newValue,fireUpdate=true)
    {
        var response = {
            blue:newValue,
        }
        var oldVal = this.leds.b;
        this.leds.b = newValue;
        this.valuesUpdated=true;
        this.forceUpdateCanvas();
        if ((oldVal != newValue) && (fireUpdate) && (this.fireOnUpdated !== undefined))
        {
            this.onupdated(response);
        }
    }
    draw()
    {
        const ctx = this.createDrawContext();
        ctx.clearRect(0, 0, this.width, this.height);
        ctx.drawImage(this.background,0,0);
        this.valuesUpdated=false;

        // draw LED

        var red = 0;
        var green = 0;
        var blue = 0;
        var ocapacity = 0;

        if (this.leds.r)
        {
            red = 255;
            ocapacity = 0.7;
        }
        if (this.leds.g)
        {
            green = 255;
            ocapacity = 0.7;
        }
        if (this.leds.b)
        {
            blue = 255;
            ocapacity = 0.7;
        }

        ctx.fillStyle = "rgba(" + red + ", " + green + ", " + blue + ", " + ocapacity + ")"
        ctx.beginPath();
        
        ctx.arc(this.position.led.x, this.position.led.y, this.position.led.radius, 0, 2 * Math.PI);
        ctx.fill();

        if (this.buttonPressed)
        {
            // draw Button
            ctx.fillStyle = "rgba(255, 255, 255, 0.7)"
            ctx.beginPath();
            ctx.arc(this.position.button.x, this.position.button.y, this.position.button.radius, 0, 2 * Math.PI);
            ctx.fill();
        }
    }
  }

if (typeof module != "undefined")
{
    module.exports.default = module.exports = {LIotBoard}
}