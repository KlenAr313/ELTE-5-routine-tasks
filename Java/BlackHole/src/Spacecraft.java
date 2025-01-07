import systems.PowerSupplier;
import systems.SpacecraftData;
import systems.primary.OxygenGenerator;
import systems.secondary.CommunicationHandler;
import systems.secondary.PropulsionController;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Spacecraft extends Thread{
    private final ExecutorService systems;
    private PowerSupplier powerSupplier;
    private SpacecraftData spacecraftData;
    private OxygenGenerator oxygenGenerator;
    private CommunicationHandler communicationHandler;
    private PropulsionController propulsionController;

    private String name;

    public Spacecraft(String name) {
        systems = Executors.newFixedThreadPool(3);
        this.name = name;
        powerSupplier = new PowerSupplier(50);
        spacecraftData = new SpacecraftData(true);
        oxygenGenerator = new OxygenGenerator(name, powerSupplier, spacecraftData);
        communicationHandler = new CommunicationHandler(name, powerSupplier, spacecraftData);
        propulsionController = new PropulsionController(name, powerSupplier, spacecraftData);

    }

    @Override
    public void run() {
        try {
            systems.execute(oxygenGenerator);
            systems.execute(communicationHandler);
            systems.execute(propulsionController);
            sleep(3000);
            systems.shutdownNow();

        }
        catch (InterruptedException e) {
            System.out.println("Spacecraft thread interrupted");
        }
    }
}
