
def buyCalc(buyRound):
    print(buyRound)
    money=10000
    moneyIncRate=2
    discount=0.9
    price=1
    totalShare=money
    totalValue=money
    totalCost=money

    for i in range(buyRound-1):
        price*=discount
        totalValue*=discount
        money*=moneyIncRate
        totalShare+=money/price
        totalValue+=money
        totalCost+=money

    print('%.6f'%totalValue, '%.6f'%price, '%.6f'%totalShare, totalCost)

    avgCost=totalCost/totalShare
    profitRatio=str(round(totalValue/totalCost-1, 4)*100)+"%"
    expectPR=str(round(1/avgCost-1, 4)*100)+"%"
    print('%.6f'%avgCost, profitRatio, expectPR)

buyCalc(4)
print()
# buyCalc(2)
