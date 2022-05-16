FROM nodered/node-red:2.2.2-12
VOLUME [ "/data" ]
# Copy package.json to the WORKDIR to install extras nodes
#CMD chown :100 /data  
#CMD chmod 775 /data 
COPY package.json .
#Install nodes
RUN npm install --unsafe-perm --no-update-notifier --no-fund --only=production
#Correct Erros
RUN npm audit fix
#copy files
COPY flows.json /data/
COPY flows_cred.json /data/
COPY settings.js /data/
#Exposing ports
EXPOSE 1880-1890
