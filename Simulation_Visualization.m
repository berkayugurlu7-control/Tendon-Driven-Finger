% Timeseries verileri (örnek olarak)
t = 0:0.001:10; % Zaman vektörü
theta1_ts = out.theta1;  % Açı 1
theta2_ts = out.theta2;  % Açı 2
theta3_ts = out.theta3;  % Açı 3

% Robot kolunun bağlantı noktalarını hesapla (eklem noktaları)
x0 = 0;
y0 = 0;

x1 = 10 * cos(theta1_ts.Data);
y1 = 10 * sin(theta1_ts.Data);

x2 = x1 + 9 * cos(theta1_ts.Data + theta2_ts.Data);
y2 = y1 + 9 * sin(theta1_ts.Data + theta2_ts.Data);

x3 = x2 + 5.7 * cos(theta1_ts.Data + theta2_ts.Data + theta3_ts.Data);
y3 = y2 + 5.7 * sin(theta1_ts.Data + theta2_ts.Data + theta3_ts.Data);

% Parmak ucunun izlediği yolu saklamak için boş vektör oluştur
x_traj = [];
y_traj = [];

% Animasyon başlatma
figure;
hold on;
h = plot([0, x0], [0, y0], 'ro-', 'MarkerFaceColor', 'r'); % Robot bağlantıları
h_traj = plot(0, 0, 'b.'); % Parmak ucunun izlediği yol
axis([-2 30 -2 30]);
grid on;
xlabel('X Konumu');
ylabel('Y Konumu');
title('Robot Parmağın Hareketi');
legend({'Parmak','İzlenen Yol'}, 'Location', 'Best');

% Animasyon döngüsü
for i = 1:length(t)
    % Robot kolunun her bir eklem noktasını güncelle
    set(h, 'XData', [0, x1(i), x2(i), x3(i)], 'YData', [0, y1(i), y2(i), y3(i)]);
    
    % Parmak ucunun izlediği yolu sakla
    x_traj = [x_traj, x3(i)];
    y_traj = [y_traj, y3(i)];
    
    % İzlenen yolu güncelle
    set(h_traj, 'XData', x_traj, 'YData', y_traj);
    
    drawnow;
end
hold off;
