import pandas as pd

# Leer el archivo CSV original con 400,000 filas
original_df = pd.read_csv('audio_features.csv')
filter = ['isrc', 'acousticness', 'danceability', 'duration_ms', 'energy', 'instrumentalness', 'key', 'liveness', 'loudness', 'mode', 'speechiness', 'tempo', 'time_signature', 'valence']

df = original_df[filter]

# Crear un nuevo DataFrame con 1,000 filas (puedes cambiar este número)
subset_df_1k = df.head(1000)

# Crear un nuevo DataFrame con 10,000 filas (puedes cambiar este número)
subset_df_10k = df.head(10000)

# Crear un nuevo DataFrame con 100,000 filas (puedes cambiar este número)
subset_df_100k = df.head(100000)

# Guardar los DataFrames en archivos CSV separados
subset_df_1k.to_csv('audio_features_1k.csv', index=False)
subset_df_10k.to_csv('audio_features_10k.csv', index=False)
subset_df_100k.to_csv('audio_features_100k.csv', index=False)

print("Archivos CSV creados con éxito.")
