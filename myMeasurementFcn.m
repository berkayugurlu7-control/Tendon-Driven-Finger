function z_pred = myMeasurementFcn(x)
    % x: durum vektörü
    % Burada ölçüm modelini yaz. Örnek:
    z1 = x(1);
    z2 = x(2);
    z3 = x(3);
    z4 = x(4);
    z5 = x(5);
    z6 = x(6);
    

    z_pred = [z1; z2; z3; z4; z5; z6];
end
