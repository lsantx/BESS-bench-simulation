%Armazena as caracteristicas da simulacao atual
mdl = plecs('get', '', 'CurrentCircuit');
%salva path do scope na raiz, se estiver dentro de
%um subsystem usar '/Sub/Scope'
%scopepath = [mdl '/Psemi'];

%carrega um valor inicial em Ih e H  A=Ih 
loadStructure = struct('Vdc_bat1', 0);
loadStructure = struct('Vdc_bat2', 0);
loadStructure = struct('Ibat1', 0);
loadStructure = struct('Ibat2', 0);

%Cria uma estrutura com as variaveis
varStructure = struct('ModelVars', loadStructure);
%Apaga o scope
%plecs('scope', scopepath, 'ClearTraces');

Vdc_bat1Vals = [187.22, 184.68, 182.22, 179.46]
Vdc_bat2Vals = [187.29, 184.44, 181.64, 178.02]
Ibat1Vals = [14.684, 14.38, 13.745, 14.329]
Ibat2Vals = [14.499, 14.569, 14.176, 14.549]

for ki = 1:length(Vdc_bat1Vals)
    varStructure.ModelVars.Vdc_bat1 = Vdc_bat1Vals(ki);
    varStructure.ModelVars.Vdc_bat2 = Vdc_bat2Vals(ki);
    varStructure.ModelVars.Ibat1 = Ibat1Vals(ki);
    varStructure.ModelVars.Ibat2 = Ibat2Vals(ki);
    Out = plecs('simulate', varStructure);
    Pchaves_inv_cond(ki) = Out.Values(1,end);
    Pchaves_inv_sw(ki) = Out.Values(2,end);
    Binv1(ki,:) = Out.Values(3,(end-(2/(60)/(1/(9000*120))):end));
    Bg1(ki,:) = Out.Values(4,(end-(2/(60)/(1/(9000*120))):end));
    Pcp_ind_LCL(ki) = Out.Values(5,end);
    P_cap_LCL(ki) = Out.Values(6,end);
    Pchaves_conv_cc_cond(ki,:) = Out.Values(7,end);
    Pchaves_conv_cc_sw(ki,:) = Out.Values(8,end);
    Pcp_ind_bt(ki,:) = Out.Values(9,end);
    Bind1(ki,:) = Out.Values(10,(end-(2/(60)/(1/(9000*120))):end));
    Pchaves_conv_cc_cond2(ki,:) = Out.Values(11,end);
    Pchaves_conv_cc_sw2(ki,:) = Out.Values(12,end);
    Bind2(ki,:) = Out.Values(13,(end-(2/(60)/(1/(9000*120))):end));
    Pcp_ind_bt2(ki,:) = Out.Values(14,end);
    Pot_grid(ki) = Out.Values(15,end);
    Pot_bat(ki) = Out.Values(16,end);
    Pot_bat2(ki) = Out.Values(17,end);
end    
                       
%salva a variavel
save("Pchaves_inv_cond.mat", "-mat", "Pchaves_inv_cond")
save("Pchaves_inv_sw.mat", "-mat", "Pchaves_inv_sw")
save("Binv1.mat", "-mat", "Binv1")
save("Bg1.mat", "-mat", "Bg1")
save("Pcp_ind_LCL.mat", "-mat", "Pcp_ind_LCL")
save("P_cap_LCL.mat", "-mat", "P_cap_LCL")
save("Pchaves_conv_cc_cond.mat", "-mat", "Pchaves_conv_cc_cond")
save("Pchaves_conv_cc_sw.mat", "-mat", "Pchaves_conv_cc_sw")
save("Pcp_ind_bt.mat", "-mat", "Pcp_ind_bt")
save("Bind1.mat", "-mat", "Bind1")
save("Pchaves_conv_cc_cond2.mat", "-mat", "Pchaves_conv_cc_cond2")
save("Pchaves_conv_cc_sw2.mat", "-mat", "Pchaves_conv_cc_sw2")
save("Bind2.mat", "-mat", "Bind2")
save("Pcp_ind_bt2.mat", "-mat", "Pcp_ind_bt2")
save("Pot_grid.mat", "-mat", "Pot_grid")
save("Pot_bat.mat", "-mat", "Pot_bat")
save("Pot_bat2.mat", "-mat", "Pot_bat2")

% Bg_f(kx,:) = Out.Values(8,(end-1/(60*1/(12000*256))):end);
% Bi_f(kx,:) = Out.Values(9,(end-1/(60*1/(12000*256))):end);