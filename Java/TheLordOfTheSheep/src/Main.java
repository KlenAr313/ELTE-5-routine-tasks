public class Main {
    public static void main(String[] args) {
        try {
            //Farm farm = new Farm(23, 32, 10, 25, 200);
            Farm farm = new Farm();
            farm.StartFarm();
        }
        catch (Exception e) {
            System.out.println(e);
        }
    }
}