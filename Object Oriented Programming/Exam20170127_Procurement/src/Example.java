import warehouse.*;

public class Example {

    public static void main(String[] args) throws InvalidSupplier, MultipleDelivery{
        Warehouse m = new Warehouse();
        
        Product banane = m.newProduct("BNN","Banane");
        banane.setQuantity(33);
        Product kiwi = m.newProduct("KWI","Kiwi");
        kiwi.setQuantity(44);
        
        Supplier chiquita = m.newSupplier("CQT", "Chiquita");
        Supplier delmonte = m.newSupplier("DMT", "Del Monte");
        
        chiquita.newSupply(banane);
        chiquita.newSupply(kiwi);
        
        delmonte.newSupply(banane);
        
        Order ord1 = m.issueOrder(banane,67,chiquita);
        Order ord2 = m.issueOrder(banane,100,delmonte);

        ord1.setDelivered();
        
        System.out.println(ord2.toString());

    }
}
