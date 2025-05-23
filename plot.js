document.getElementById('graphForm').addEventListener('submit', function (e) {
  e.preventDefault();
  const eq = encodeURIComponent(document.getElementById('equation').value);

  fetch(`/cgi-bin/graph.cgi?expr=${encodeURIComponent(eq)}`)

    .then(res => res.text())
    .then(data => {
      const x = [], y = [];
      data.trim().split("\n").forEach(line => {
        const [xVal, yVal] = line.split(" ");
        x.push(parseFloat(xVal));
        y.push(parseFloat(yVal));
      });

      Plotly.newPlot('plot', [{
        x: x,
        y: y,
        mode: 'lines',
        type: 'scatter'
      }]);
    });
});


