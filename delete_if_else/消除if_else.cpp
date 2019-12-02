

//接口
public interface UserPayService {

    /**
     * 计算应付价格
     */
    public BigDecimal quote(BigDecimal orderPrice);
}


//策略类

/**
 * @author mhcoding
 */
public class ParticularlyVipPayService implements UserPayService {

    @Override
    public BigDecimal quote(BigDecimal orderPrice) {
         if (消费金额大于30元) {
            return 7折价格;
        }
    }
}

public class SuperVipPayService implements UserPayService {

    @Override
    public BigDecimal quote(BigDecimal orderPrice) {
        return 8折价格;
    }
}

public class VipPayService implements UserPayService {

    @Override
    public BigDecimal quote(BigDecimal orderPrice) {
        if(该用户超级会员刚过期并且尚未使用过临时折扣){
            临时折扣使用次数更新();
            returen 8折价格;
        }
        return 9折价格;
    }
}


/**
 * @author mhcoding
 */
public class Test {

    public static void main(String[] args) {
        UserPayService strategy = new VipPayService();
        BigDecimal quote = strategy.quote(300);
        System.out.println("普通会员商品的最终价格为：" + quote.doubleValue());

        strategy = new SuperVipPayService();
        quote = strategy.quote(300);
        System.out.println("超级会员商品的最终价格为：" + quote.doubleValue());
    }
}


//提供工厂类的注册类，利用map保存
public class UserPayServiceStrategyFactory {

    private static Map<String,UserPayService> services = new ConcurrentHashMap<String,UserPayService>();

    public  static UserPayService getByUserType(String type){
        return services.get(type);
    }

    public static void register(String userType,UserPayService userPayService){
        Assert.notNull(userType,"userType can't be null");
        services.put(userType,userPayService);
    }
}


@Service
public class ParticularlyVipPayService implements UserPayService,InitializingBean {

    @Override
    public BigDecimal quote(BigDecimal orderPrice) {
         if (消费金额大于30元) {
            return 7折价格;
        }
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        UserPayServiceStrategyFactory.register("ParticularlyVip",this);
    }
}

@Service
public class SuperVipPayService implements UserPayService ,InitializingBean{

    @Override
    public BigDecimal quote(BigDecimal orderPrice) {
        return 8折价格;
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        UserPayServiceStrategyFactory.register("SuperVip",this);
    }
}

@Service  
public class VipPayService implements UserPayService,InitializingBean {

    @Override
    public BigDecimal quote(BigDecimal orderPrice) {
        if(该用户超级会员刚过期并且尚未使用过临时折扣){
            临时折扣使用次数更新();
            returen 8折价格;
        }
        return 9折价格;
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        UserPayServiceStrategyFactory.register("Vip",this);
    }
}