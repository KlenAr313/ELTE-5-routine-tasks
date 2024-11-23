public class Sheep implements Animal {
    public char name;
    private int x;
    private int y;
    private int waitTime;
    private Boolean escaped = false;

    public Sheep(int name, int x, int y, int waitTime) {
        this.name = (char)((name) % 25 + 65);
        this.x = x;
        this.y = y;
        this.escaped = escaped;
        this.waitTime = waitTime;
    }

    public void Start(Farm farm) {
        while(!escaped) {
            try{

                if(x == 0 || y == 0 || x == farm.length - 1 || y == farm.length - 1) {
                    farm.SheepEscaped(name);
                    break;
                }

                Thread.sleep(waitTime);
            }
            catch(InterruptedException e){
                e.printStackTrace();
            }
        }
    }

    public void SwitchEscapedStatus() {
        escaped = true;
    }

    @Override
    public boolean IsDog() {
        return false;
    }

    @Override
    public String toString() {
        return Character.toString(name);
    }
}
