const dev = 'https://9vsql7oa2m.execute-api.us-east-1.amazonaws.com/dev/particle/publishEvent'
const prod = 'https://fkuy8c5zz4.execute-api.us-east-1.amazonaws.com/prod/particle/publishEvent'

const env = 'dev'

let url
if (env === 'prod') {
  url = prod
} else {
  url = dev
}

$('.toggleOutlet').on('click', async function () {
  var dataParams = $(this).attr('data-outletStatus') + ',' + $(this).attr('data-outletLocation') + ',' + $(this).attr('data-outletId');
  await $.ajax({
    method: 'POST',
    url,
    headers: {
      'Access-Control-Allow-Origin': '*',
      'Access-Control-Allow-Methods': 'POST, OPTIONS',
      'Access-Control-Allow-Credentials': 'true'
    },
    data: JSON.stringify({
      action: 'switchLightsState',
      params: dataParams,
      secretId: 'prod/lightboard/particle'
    }),
    processData: false
  }).catch((e) => {
    console.log(e)
  })
});
