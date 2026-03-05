import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import numpy as np
matplotlib.rcParams['font.family'] = 'DejaVu Sans'
matplotlib.rcParams['axes.unicode_minus'] = False

df = pd.read_csv('results/resultados_benchmark.csv')
df.columns = df.columns.str.strip()

ALGOS  = ['BubbleSort','InsertionSort','SelectionSort','QuickSort','MergeSort']
TIPOS  = ['Aleatorio','Parcialmente_Ordenado','Inversamente_Ordenado']
LABELS = ['Aleatório','Parc. Ordenado','Inv. Ordenado']
NS     = [100, 1000, 10000]
CORES  = ['#e41a1c','#377eb8','#4daf4a','#ff7f00','#984ea3']
ESTILO = ['-o','-s','-^','-D','-*']

din = df[df['Estrutura']=='Lista_Dinamica']
est = df[df['Estrutura']=='Lista_Estatica']

# ── Fig 1: Tempo x N (entrada aleatória, lista dinâmica) ──────────────
fig, ax = plt.subplots(figsize=(6,4))
for i, a in enumerate(ALGOS):
    sub = din[(din['Algoritmo']==a)&(din['Tipo_Entrada']=='Aleatorio')].sort_values('N')
    ax.plot(sub['N'], sub['Tempo_Medio_ms'], ESTILO[i], color=CORES[i], label=a, markersize=6)
ax.set_xscale('log'); ax.set_yscale('log')
ax.set_xlabel('N'); ax.set_ylabel('Tempo médio (ms)')
ax.set_title('Tempo x N — Entrada Aleatória (Lista Dinâmica)')
ax.legend(fontsize=8); ax.grid(True, which='both', alpha=0.3)
plt.tight_layout(); plt.savefig('artigo/fig1_tempo_vs_n.png', dpi=150); plt.close()

# ── Fig 2: Comparações x N ─────────────────────────────────────────────
fig, ax = plt.subplots(figsize=(6,4))
for i, a in enumerate(ALGOS):
    sub = din[(din['Algoritmo']==a)&(din['Tipo_Entrada']=='Aleatorio')].sort_values('N')
    ax.plot(sub['N'], sub['Comparacoes_Medias'].abs(), ESTILO[i], color=CORES[i], label=a, markersize=6)
ax.set_xscale('log'); ax.set_yscale('log')
ax.set_xlabel('N'); ax.set_ylabel('Comparações médias')
ax.set_title('Comparações x N — Entrada Aleatória')
ax.legend(fontsize=8); ax.grid(True, which='both', alpha=0.3)
plt.tight_layout(); plt.savefig('artigo/fig2_comparacoes.png', dpi=150); plt.close()

# ── Fig 3: Heatmap N=10000 ──────────────────────────────────────────────
fig, ax = plt.subplots(figsize=(6,4))
mat = np.zeros((5,3))
for i, a in enumerate(ALGOS):
    for j, t in enumerate(TIPOS):
        v = din[(din['Algoritmo']==a)&(din['Tipo_Entrada']==t)&(din['N']==10000)]['Tempo_Medio_ms']
        mat[i,j] = v.values[0] if len(v) else 0
im = ax.imshow(mat, cmap='RdYlGn_r', aspect='auto')
ax.set_xticks(range(3)); ax.set_xticklabels(LABELS, fontsize=8)
ax.set_yticks(range(5)); ax.set_yticklabels(ALGOS, fontsize=8)
for i in range(5):
    for j in range(3):
        ax.text(j, i, f'{mat[i,j]:.1f}', ha='center', va='center', fontsize=7,
                color='white' if mat[i,j] > 200 else 'black')
plt.colorbar(im, ax=ax, label='ms')
ax.set_title('Heatmap de Tempo (ms) — N=10.000')
plt.tight_layout(); plt.savefig('artigo/fig3_heatmap.png', dpi=150); plt.close()

# ── Fig 4: Quick vs Merge — dinâmica e estática ────────────────────────
fig, ax = plt.subplots(figsize=(7,4))
x = np.arange(3); w = 0.18
configs = [
    ('QuickSort', 'Lista_Dinamica', '#ff7f00', 0.6, 'Quick (Din.)'),
    ('QuickSort', 'Lista_Estatica', '#ff7f00', 1.0, 'Quick (Est.)'),
    ('MergeSort', 'Lista_Dinamica', '#984ea3', 0.6, 'Merge (Din.)'),
    ('MergeSort', 'Lista_Estatica', '#984ea3', 1.0, 'Merge (Est.)'),
]
for k, (algo, struct, cor, alpha, lbl) in enumerate(configs):
    vals = []
    for t in TIPOS:
        v = df[(df['Algoritmo']==algo)&(df['Tipo_Entrada']==t)&(df['N']==10000)&(df['Estrutura']==struct)]['Tempo_Medio_ms']
        vals.append(v.values[0] if len(v) else 0)
    ax.bar(x + (k-1.5)*w, vals, w, label=lbl, color=cor, alpha=alpha)
ax.set_xticks(x); ax.set_xticklabels(LABELS, fontsize=8)
ax.set_ylabel('Tempo médio (ms)')
ax.set_title('Quick Sort vs Merge Sort — N=10.000 (Dinâmica e Estática)')
ax.legend(fontsize=7); ax.grid(axis='y', alpha=0.3)
plt.tight_layout(); plt.savefig('artigo/fig4_quick_vs_merge.png', dpi=150); plt.close()

# ── Fig 5: Dinâmica vs Estática por algoritmo ──────────────────────────
fig, ax = plt.subplots(figsize=(7,4))
x = np.arange(5); w = 0.35
vals_din = [din[(din['Algoritmo']==a)&(din['Tipo_Entrada']=='Aleatorio')&(din['N']==10000)]['Tempo_Medio_ms'].values[0] for a in ALGOS]
vals_est = [est[(est['Algoritmo']==a)&(est['Tipo_Entrada']=='Aleatorio')&(est['N']==10000)]['Tempo_Medio_ms'].values[0] for a in ALGOS]
ax.bar(x-w/2, vals_din, w, label='Lista Dinâmica', color='#377eb8')
ax.bar(x+w/2, vals_est, w, label='Lista Estática',  color='#e41a1c')
ax.set_xticks(x); ax.set_xticklabels(ALGOS, rotation=15, fontsize=8)
ax.set_ylabel('Tempo médio (ms)')
ax.set_title('Dinâmica vs Estática — N=10.000, Entrada Aleatória')
ax.legend(); ax.grid(axis='y', alpha=0.3)
plt.tight_layout(); plt.savefig('artigo/fig5_estruturas.png', dpi=150); plt.close()

# ── Fig 6: Ganho % da estática sobre a dinâmica ────────────────────────
fig, ax = plt.subplots(figsize=(7,4))
x = np.arange(5); w = 0.25
ns_labels = ['N=100','N=1.000','N=10.000']
for k, n in enumerate(NS):
    ganhos = []
    for a in ALGOS:
        d = din[(din['Algoritmo']==a)&(din['Tipo_Entrada']=='Aleatorio')&(din['N']==n)]['Tempo_Medio_ms'].values
        e = est[(est['Algoritmo']==a)&(est['Tipo_Entrada']=='Aleatorio')&(est['N']==n)]['Tempo_Medio_ms'].values
        ganhos.append((d[0]-e[0])/d[0]*100 if len(d) and len(e) and d[0] > 0 else 0)
    bars = ax.bar(x + (k-1)*w, ganhos, w, label=ns_labels[k])
    if n == 10000:
        for bar, g in zip(bars, ganhos):
            ax.text(bar.get_x()+bar.get_width()/2,
                    bar.get_height() + (1 if g >= 0 else -3),
                    f'{g:+.1f}%', ha='center', va='bottom', fontsize=6)
ax.axhline(0, color='black', linewidth=0.8)
ax.set_xticks(x); ax.set_xticklabels(ALGOS, rotation=15, fontsize=8)
ax.set_ylabel('Ganho (%) da Estática sobre Dinâmica')
ax.set_title('Vantagem da Lista Estática — Entrada Aleatória\n(positivo = estática mais rápida)')
ax.legend(); ax.grid(axis='y', alpha=0.3)
plt.tight_layout(); plt.savefig('artigo/fig6_ganho.png', dpi=150); plt.close()

print("6 figuras geradas em artigo/")
